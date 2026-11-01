// SPDX-License-Identifier: GPL-3.0-or-later
#include "src/core/core.h"
#include "src/persistence/profile.h"
#include "src/persistence/profilelocker.h"
#include "src/persistence/settings.h"
#include "src/persistence/settingsserializer.h"
#include "src/persistence/personalsettingsupgrader.h"
#include "src/video/camerasource.h"
#include "src/widget/tool/imessageboxmanager.h"
#include <QTemporaryDir>
#include <QTest>
#include <QFile>
#include <QSettings>
#include "src/nexus.h"
#include "src/ipc.h"
#include "src/widget/loginscreen.h"
#include "src/widget/style.h"
#include "src/model/profile/profileinfo.h"
#include <QPointer>
#include <QTimer>
#include <QMainWindow>
#include <QDir>
class MockMessageBoxManager : public IMessageBoxManager
{
public:
    ~MockMessageBoxManager() override;
    void showInfo(const QString& title, const QString& msg) override
    {
        std::ignore = title;
        std::ignore = msg;
    }
    void showWarning(const QString& title, const QString& msg) override
    {
        std::ignore = title;
        std::ignore = msg;
    }
    void showError(const QString& title, const QString& msg) override
    {
        std::ignore = title;
        std::ignore = msg;
    }
    bool askQuestion(const QString& title, const QString& msg, bool defaultAns, bool warning,
                     bool yesno) override
    {
        std::ignore = title;
        std::ignore = msg;
        std::ignore = defaultAns;
        std::ignore = warning;
        std::ignore = yesno;
        return true;
    }
    bool askQuestion(const QString& title, const QString& msg, const QString& button1,
                     const QString& button2, bool defaultAns, bool warning) override
    {
        std::ignore = title;
        std::ignore = msg;
        std::ignore = button1;
        std::ignore = button2;
        std::ignore = defaultAns;
        std::ignore = warning;
        return true;
    }
    void confirmExecutableOpen(const QFileInfo& file) override
    {
        std::ignore = file;
    }
};

MockMessageBoxManager::~MockMessageBoxManager() = default;


class AccountsTest : public QObject
{
    Q_OBJECT
private slots:
    void isolationAndRestart();
    void settingsMigration();
    void switchWithoutRestart();
};

void AccountsTest::isolationAndRestart()
{
    QTemporaryDir root;
    QVERIFY(root.isValid());
    MockMessageBoxManager boxes;
    Settings settings(boxes, Paths::Portable::Portable);
    settings.getPaths().setPortablePath(root.path());
    CameraSource camera(settings);
    const ToxPk contact(QByteArray(32, 'A'));
    auto alice = Profile::createProfile("alice", "alice-password", settings, nullptr, camera, boxes);
    QVERIFY(alice);
    const auto aliceId = alice->getCore().getSelfId();
    alice->getCore().acceptFriendRequest(contact);
    settings.setContactNote(contact, "Alice private note");
    settings.setNotify(false);
    settings.setProxyAddr("alice-proxy.invalid");
    ChatState state;
    state.archived = true;
    state.muted = true;
    state.deleted = true;
    state.pinOrder = 123456;
    state.unread = 37;
    settings.setChatState(contact, state);
    alice->save();
    settings.detachProfile();
    alice.reset();
    QVERIFY(!ProfileLocker::hasLock());

    auto bob = Profile::createProfile("bob", QString(), settings, nullptr, camera, boxes);
    QVERIFY(bob);
    const auto bobId = bob->getCore().getSelfId();
    QVERIFY(settings.getGlobalAutoAcceptDir().endsWith("files/bob"));
    QVERIFY(settings.getPaths().getUserNodesFilePath().contains("nodes/bob/"));
    QVERIFY(bobId != aliceId);
    QCOMPARE(bob->getCore().getFriendList().size(), 0);
    QCOMPARE(settings.getContactNote(contact), QString());
    QCOMPARE(settings.getChatState(contact).unread, 0);
    QVERIFY(!settings.getChatState(contact).muted);
    QVERIFY(settings.getNotify());
    QCOMPARE(settings.getProxyAddr(), QString());
    settings.setContactNote(contact, "Bob private note");
    bob->save();
    settings.detachProfile();
    bob.reset();

    QVERIFY(!Profile::loadProfile("alice", "wrong password", settings, nullptr, camera, boxes));
    QVERIFY(!ProfileLocker::hasLock());
    alice = Profile::loadProfile("alice", "alice-password", settings, nullptr, camera, boxes);
    QVERIFY(alice);
    QCOMPARE(alice->getCore().getSelfId(), aliceId);
    QCOMPARE(alice->getCore().getFriendList().size(), 1);
    QCOMPARE(settings.getContactNote(contact), QString("Alice private note"));
    QVERIFY(settings.getGlobalAutoAcceptDir().endsWith("files/alice"));
    QVERIFY(settings.getPaths().getUserNodesFilePath().contains("nodes/alice/"));
    QVERIFY(settings.getChatState(contact) == state);
    QVERIFY(!settings.getNotify());
    QCOMPARE(settings.getProxyAddr(), QString("alice-proxy.invalid"));
    QFile stored(root.filePath("alice.ini"));
    QVERIFY(stored.open(QIODevice::ReadOnly));
    const auto encrypted = stored.readAll();
    QVERIFY(ToxEncrypt::isEncrypted(encrypted));
    QVERIFY(!encrypted.contains("Alice private note"));
    {
        IPC ipc(9876);
        Nexus nexus(settings, boxes, camera, ipc);
        ProfileInfo info(&alice->getCore(), alice.get(), settings, nexus);
        QCOMPARE(info.exportProfile(root.filePath("backup.tox")), IProfileInfo::SaveResult::OK);
        QFile backup(root.filePath("backup.tox"));
        QVERIFY(backup.open(QIODevice::ReadOnly));
        QVERIFY(ToxEncrypt::isEncrypted(backup.readAll()));
        // An unwritable destination (directory) must not remove the live identity.
        QVERIFY(info.exportProfile(root.path()) != IProfileInfo::SaveResult::OK);
        QVERIFY(QFile::exists(root.filePath("alice.tox")));
    }
    alice->save();
    settings.detachProfile();
    alice.reset();
    bob = Profile::loadProfile("bob", QString(), settings, nullptr, camera, boxes);
    QVERIFY(bob);
    QCOMPARE(bob->getCore().getSelfId(), bobId);
    QCOMPARE(settings.getContactNote(contact), QString("Bob private note"));
    QCOMPARE(settings.getChatState(contact).unread, 0);
    bob->save();
    settings.detachProfile();
    bob.reset();
}

void AccountsTest::settingsMigration()
{
    QTemporaryDir root;
    SettingsSerializer old(root.filePath("old.ini"));
    old.beginGroup("Privacy");
    old.setValue("enableLogging", false);
    old.endGroup();
    QVERIFY(PersonalSettingsUpgrader::doUpgrade(old, 1, 2));
    old.beginGroup("Privacy");
    QVERIFY(!old.value("enableLogging", true).toBool());
    old.endGroup();
    QVERIFY(!PersonalSettingsUpgrader::doUpgrade(old, 3, 2));
}

void AccountsTest::switchWithoutRestart()
{
    Q_INIT_RESOURCE(translations);
    QTemporaryDir root;
    MockMessageBoxManager boxes;
    Settings settings(boxes, Paths::Portable::Portable);
    settings.getPaths().setPortablePath(root.path());
    settings.setShowSystemTray(false);
    settings.setThemeColor(12);
    CameraSource camera(settings);
    IPC ipc(12345);
    Nexus nexus(settings, boxes, camera, ipc);
    const QString screenshots = qEnvironmentVariable("PARLEY_SCREENSHOT_DIR");
    if (!screenshots.isEmpty()) {
        QDir().mkpath(screenshots);
        Style style;
        LoginScreen login(settings.getPaths(), style, settings.getThemeColor());
        login.show();
        QTest::qWait(50);
        QVERIFY(login.grab().save(screenshots + "/parley-account-switcher.png"));
    }
    QTimer::singleShot(0, &nexus, [&] { nexus.onCreateNewProfile("switch-alice", QString()); });
    QCOMPARE(nexus.showLogin(), int(QDialog::Accepted));
    QVERIFY(nexus.getProfile());
    QPointer<Core> oldCore(&nexus.getProfile()->getCore());
    const auto aliceId = oldCore->getSelfId();
    const ToxPk contact(QByteArray(32, 'X'));
    oldCore->acceptFriendRequest(contact);
    settings.setContactNote(contact, "only Alice");
    settings.setChatState(contact, ChatState{true, true, true, 123, 9});
    QTest::qWait(100);
    if (!screenshots.isEmpty()) {
        for (auto* top : QApplication::topLevelWidgets()) {
            if (auto* main = qobject_cast<QMainWindow*>(top); main && main->isVisible()) {
                main->resize(1160, 780);
                QMetaObject::invokeMethod(main, "onShowSettings", Qt::DirectConnection);
                QTest::qWait(100);
                QVERIFY(main->grab().save(screenshots + "/parley-settings.png"));
            }
        }
    }
    bool coreDestroyedBeforeLogin = false;
    QTimer::singleShot(0, &nexus, [&] {
        coreDestroyedBeforeLogin = oldCore.isNull();
        nexus.onCreateNewProfile("switch-bob", QString());
    });
    QCOMPARE(nexus.showLogin(), int(QDialog::Accepted));
    QVERIFY(coreDestroyedBeforeLogin);
    QVERIFY(nexus.getProfile());
    QVERIFY(nexus.getProfile()->getCore().getSelfId() != aliceId);
    QCOMPARE(settings.getContactNote(contact), QString());
    QCOMPARE(settings.getChatState(contact).unread, 0);
    QCOMPARE(nexus.getProfile()->getCore().getFriendList().size(), 0);
    QTimer::singleShot(0, &nexus, [&] { nexus.onLoadProfile("switch-alice", QString()); });
    QCOMPARE(nexus.showLogin(), int(QDialog::Accepted));
    QCOMPARE(nexus.getProfile()->getCore().getSelfId(), aliceId);
    QCOMPARE(settings.getContactNote(contact), QString("only Alice"));
    QCOMPARE(settings.getChatState(contact).unread, 9);
}

QTEST_MAIN(AccountsTest)
#include "accounts_test.moc"
