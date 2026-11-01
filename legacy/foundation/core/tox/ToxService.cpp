#include "ToxService.h"
#include <QByteArray>
#include <QElapsedTimer>
#include <QLoggingCategory>
#include <algorithm>
#include <array>
#include <memory>
#include <toxcore/tox.h>
#include <toxcore/tox_options.h>

Q_LOGGING_CATEGORY(toxLog, "hau.tox")
namespace {
struct Node { const char *host; uint16_t port; const char *key; };
// Snapshot of active nodes from https://nodes.tox.chat/json; replace as nodes retire.
constexpr Node nodes[] = {
    {"tox.abilinski.com", 33445, "10C00EB250C3233E343E2AEBA07115A5C28920E9C8D29492F6D00B29049EDC7E"},
    {"tox1.mf-net.eu", 33445, "B3E5FA80DC8EBD1149AD2AB35ED8B85BD546DEDE261CA593234C619249419506"},
    {"tox.initramfs.io", 33445, "3F0A45A268367C1BEA652F258C85F4A66DA76BCAA667A49E770BCC4917AB6A25"},
    {"144.217.167.73", 33445, "7E5668E0EE09E19F320AD47902419331FFEE147BB3606769CFBE921A2A2FD34C"},
};
using ToxPtr = std::unique_ptr<Tox, decltype(&tox_kill)>;
using OptionsPtr = std::unique_ptr<Tox_Options, decltype(&tox_options_free)>;

bool persist(Tox *tox, const Profile &profile, QString &error) {
    const auto length = tox_get_savedata_size(tox);
    if (length == 0 || length > 32 * 1024 * 1024) { error = QStringLiteral("Invalid identity size"); return false; }
    QByteArray data(static_cast<qsizetype>(length), Qt::Uninitialized);
    tox_get_savedata(tox, reinterpret_cast<uint8_t *>(data.data()));
    const bool ok = ProfileStore::writeSavedata(profile, data, error);
    data.fill('\0');
    return ok;
}

bool bootstrap(Tox *tox) {
    bool any = false;
    for (const auto &node : nodes) {
        const auto key = QByteArray::fromHex(node.key);
        if (key.size() != TOX_PUBLIC_KEY_SIZE) continue;
        Tox_Err_Bootstrap error{};
        if (tox_bootstrap(tox, node.host, node.port,
                          reinterpret_cast<const uint8_t *>(key.constData()), &error)) any = true;
        else qCWarning(toxLog) << "Bootstrap request failed for" << node.host << "error" << int(error);
    }
    return any;
}
}

ToxService::ToxService(Profile profile, QObject *parent, bool enableBootstrap)
    : QThread(parent), profile_(std::move(profile)), enableBootstrap_(enableBootstrap) {}

ToxService::~ToxService() { stop(); }

void ToxService::stop() {
    stopping_.store(true);
    if (isRunning() && QThread::currentThread() != this) wait();
}

void ToxService::run() {
    QByteArray saved;
    QString error;
    if (!profile_.isNew && !ProfileStore::readSavedata(profile_, saved, error)) {
        emit failure(QStringLiteral("Cannot read the existing identity. Original data was preserved.")); return;
    }
    Tox_Err_Options_New optionsError{};
    OptionsPtr options(tox_options_new(&optionsError), &tox_options_free);
    if (!options) { emit failure(QStringLiteral("Cannot initialize Tox options")); return; }
    if (!saved.isEmpty()) {
        tox_options_set_savedata_type(options.get(), TOX_SAVEDATA_TYPE_TOX_SAVE);
        if (!tox_options_set_savedata_data(options.get(), reinterpret_cast<const uint8_t *>(saved.constData()),
                                           static_cast<size_t>(saved.size()))) {
            saved.fill('\0'); emit failure(QStringLiteral("Cannot load profile identity")); return;
        }
    }
    Tox_Err_New newError{};
    ToxPtr tox(tox_new(options.get(), &newError), &tox_kill);
    saved.fill('\0');
    options.reset();
    if (!tox) {
        qCWarning(toxLog) << "tox_new failed" << int(newError);
        emit failure(QStringLiteral("Tox identity is invalid or networking could not start. Identity was preserved."));
        return;
    }
    if (profile_.isNew && !persist(tox.get(), profile_, error)) {
        qCWarning(toxLog) << "Initial identity save failed" << error;
        emit failure(QStringLiteral("Cannot safely save the new identity")); return;
    }
    std::array<uint8_t, TOX_ADDRESS_SIZE> address{};
    tox_self_get_address(tox.get(), address.data());
    emit identityReady(ProfileStore::encodeAddress(QByteArray(reinterpret_cast<const char *>(address.data()), address.size())));
    emit networkStateChanged(1);
    if (enableBootstrap_ && !bootstrap(tox.get())) qCWarning(toxLog) << "No bootstrap requests succeeded; will retry";
    QElapsedTimer savedTimer, bootstrapTimer;
    savedTimer.start(); bootstrapTimer.start();
    int lastState = 1;
    while (!stopping_.load()) {
        tox_iterate(tox.get(), nullptr);
        const int state = tox_self_get_connection_status(tox.get()) == TOX_CONNECTION_NONE ? 1 : 2;
        if (state != lastState) { lastState = state; emit networkStateChanged(state); }
        if (enableBootstrap_ && bootstrapTimer.elapsed() >= 60000 && state == 1) { bootstrap(tox.get()); bootstrapTimer.restart(); }
        if (savedTimer.elapsed() >= 30000) {
            if (!persist(tox.get(), profile_, error)) qCWarning(toxLog) << "Periodic identity save failed" << error;
            savedTimer.restart();
        }
        const auto interval = std::clamp(tox_iteration_interval(tox.get()), uint32_t(10), uint32_t(100));
        QThread::msleep(interval);
    }
    if (!persist(tox.get(), profile_, error)) {
        qCWarning(toxLog) << "Final identity save failed" << error;
        emit failure(QStringLiteral("Could not save the latest identity state"));
    }
    emit networkStateChanged(0);
}
