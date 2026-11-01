// SPDX-License-Identifier: GPL-3.0-or-later
// Actual Parley Core against a plain c-toxcore peer. No simulated transport.
#include "src/core/core.h"
#include "src/core/corefile.h"
#include "mock/mockcoresettings.h"
#include "mock/mockbootstraplistgenerator.h"
#include <tox/tox.h>
#include <QElapsedTimer>
#include <QFile>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QTest>
#include <array>
#include <algorithm>
#include <memory>

class TransportTest : public QObject
{
    Q_OBJECT
private slots:
    void privateMessagesAndConference();
};

void TransportTest::privateMessagesAndConference()
{
    MockSettings settings;
    settings.setProxyType(ICoreSettings::ProxyType::ptNone);
    settings.setProxyPort(0);
    MockBootstrapListGenerator nodes;
    const std::unique_ptr<Tox_Options, decltype(&tox_options_free)> options(tox_options_new(nullptr), tox_options_free);
    tox_options_set_ipv6_enabled(options.get(), false);
    tox_options_set_local_discovery_enabled(options.get(), false);
    const std::unique_ptr<Tox, decltype(&tox_kill)> peer(tox_new(options.get(), nullptr), tox_kill);
    QVERIFY(peer);
    auto [core, error] = Core::makeToxCore({}, settings, nodes);
    QVERIFY(core);
    std::array<uint8_t, TOX_PUBLIC_KEY_SIZE> peerPk{};
    tox_self_get_public_key(peer.get(), peerPk.data());
    auto corePk = core->getSelfPublicKey();
    const uint32_t peerFriend = tox_friend_add_norequest(peer.get(), corePk.getData(), nullptr);
    core->acceptFriendRequest(ToxPk(peerPk.data()));
    const uint32_t coreFriend = core->getFriendList().first();
    // Plain peer bootstraps directly to Parley's real DHT UDP endpoint.
    const auto dht = core->getSelfDhtId();
    QVERIFY(tox_bootstrap(peer.get(), "127.0.0.1", core->getSelfUdpPort(),
        reinterpret_cast<const uint8_t*>(dht.constData()), nullptr));
    core->start();
    const auto waitFor = [&](auto predicate, int timeout = 20000) {
        QElapsedTimer timer; timer.start();
        while (!predicate() && timer.elapsed() < timeout) {
            tox_iterate(peer.get(), nullptr);
            QTest::qWait(10);
        }
        return predicate();
    };
    QVERIFY2(waitFor([&] { return tox_friend_get_connection_status(peer.get(), peerFriend, nullptr) != TOX_CONNECTION_NONE && core->isFriendOnline(coreFriend); }), "Local peer never connected");
    struct Received { QByteArray text; uint32_t conference = UINT32_MAX; QByteArray conferenceText;
        QByteArray fileData;
        uint32_t offeredFile = UINT32_MAX;
        bool fileFinished = false;
        bool acceptFiles = true;
        bool remoteCancelled = false;
    } received;
    tox_callback_friend_message(peer.get(), +[](Tox*, uint32_t, Tox_Message_Type, const uint8_t* text, size_t size, void* data) {
        static_cast<Received*>(data)->text = QByteArray(reinterpret_cast<const char*>(text), size);
    });
    // Pass callback state in all subsequent peer iterations.
    const auto pumpFor = [&](auto predicate, int timeout = 10000) {
        QElapsedTimer timer; timer.start();
        while (!predicate() && timer.elapsed() < timeout) {
            tox_iterate(peer.get(), &received);
            QTest::qWait(10);
        }
        return predicate();
    };
    ReceiptNum receipt;
    QVERIFY(core->sendMessage(coreFriend, "Parley to ordinary Tox peer", receipt));
    QVERIFY(pumpFor([&] { return !received.text.isEmpty(); }));
    QCOMPARE(received.text, QByteArray("Parley to ordinary Tox peer"));
    QSignalSpy inbound(core.get(), &Core::friendMessageReceived);
    const QByteArray response("ordinary Tox peer to Parley");
    tox_friend_send_message(peer.get(), peerFriend, TOX_MESSAGE_TYPE_NORMAL,
        reinterpret_cast<const uint8_t*>(response.constData()), response.size(), nullptr);
    QVERIFY(pumpFor([&] { return inbound.count() == 1; }));
    QCOMPARE(inbound[0][1].toString(), QString::fromUtf8(response));

    tox_callback_conference_invite(peer.get(), +[](Tox* tox, uint32_t friendNum, Tox_Conference_Type, const uint8_t* cookie, size_t size, void* data) {
        static_cast<Received*>(data)->conference = tox_conference_join(tox, friendNum, cookie, size, nullptr);
    });
    tox_callback_conference_message(peer.get(), +[](Tox*, uint32_t, uint32_t, Tox_Message_Type, const uint8_t* text, size_t size, void* data) {
        static_cast<Received*>(data)->conferenceText = QByteArray(reinterpret_cast<const char*>(text), size);
    });
    const int conference = core->createConference(TOX_CONFERENCE_TYPE_TEXT);
    QVERIFY(conference >= 0);
    core->conferenceInviteFriend(coreFriend, conference);
    QVERIFY(pumpFor([&] { return received.conference != UINT32_MAX; }));
    QVERIFY(pumpFor([&] { return core->getConferenceNumberPeers(conference) >= 2; }));
    core->sendConferenceMessage(conference, "conference interoperability");
    QVERIFY(pumpFor([&] { return !received.conferenceText.isEmpty(); }));
    QCOMPARE(received.conferenceText, QByteArray("conference interoperability"));

    // Actual file bytes and cancellation between Parley's transfer engine and plain toxcore.
    tox_callback_file_recv(peer.get(), +[](Tox* tox, uint32_t friendNum, uint32_t fileNum,
        uint32_t kind, uint64_t, const uint8_t*, size_t, void* data) {
        if (kind != TOX_FILE_KIND_DATA) return;
        auto* state = static_cast<Received*>(data);
        state->offeredFile = fileNum;
        if (state->acceptFiles) tox_file_control(tox, friendNum, fileNum, TOX_FILE_CONTROL_RESUME, nullptr);
    });
    tox_callback_file_recv_chunk(peer.get(), +[](Tox*, uint32_t, uint32_t, uint64_t position,
        const uint8_t* bytes, size_t size, void* data) {
        auto* state = static_cast<Received*>(data);
        if (!size) { state->fileFinished = true; return; }
        if (state->fileData.size() < static_cast<qsizetype>(position + size))
            state->fileData.resize(position + size);
        std::copy(bytes, bytes + size, state->fileData.begin() + position);
    });
    tox_callback_file_recv_control(peer.get(), +[](Tox*, uint32_t, uint32_t, Tox_File_Control control, void* data) {
        if (control == TOX_FILE_CONTROL_CANCEL) static_cast<Received*>(data)->remoteCancelled = true;
    });
    QTemporaryDir files;
    const QByteArray payload(128 * 1024, 'P');
    QFile source(files.filePath("payload.bin"));
    QVERIFY(source.open(QIODevice::WriteOnly));
    QCOMPARE(source.write(payload), qint64(payload.size()));
    source.close();
    auto* transfers = core->getCoreFile();
    QSignalSpy finished(transfers, &CoreFile::fileTransferFinished);
    QSignalSpy started(transfers, &CoreFile::fileSendStarted);
    transfers->sendFile(coreFriend, "payload.bin", source.fileName(), payload.size());
    QVERIFY(pumpFor([&] { return received.fileFinished && !finished.isEmpty(); }));
    QCOMPARE(received.fileData, payload);
    received.acceptFiles = false;
    received.offeredFile = UINT32_MAX;
    QSignalSpy cancelled(transfers, &CoreFile::fileTransferCancelled);
    transfers->sendFile(coreFriend, "cancel-me.bin", source.fileName(), payload.size());
    QVERIFY(pumpFor([&] { return received.offeredFile != UINT32_MAX && started.count() >= 2; }));
    const auto offer = qvariant_cast<ToxFile>(started.last().first());
    transfers->cancelFileSend(coreFriend, offer.fileNum);
    QVERIFY(pumpFor([&] { return !cancelled.isEmpty() && received.remoteCancelled; }));
}
QTEST_MAIN(TransportTest)
#include "transport_test.moc"
