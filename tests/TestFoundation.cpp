#include "core/profile/ProfileStore.h"
#include "core/storage/Database.h"
#include "core/tox/ToxService.h"
#include <QFile>
#include <QSignalSpy>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTemporaryDir>
#include <QtTest>

class FoundationTest : public QObject {
    Q_OBJECT
private slots:
    void profileAndMetadata();
    void malformedMetadata();
    void atomicSavedata();
    void migrations();
    void addressEncoding();
    void identityRestartAndStress();
};

void FoundationTest::profileAndMetadata() {
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    ProfileStore store(dir.path()); Profile first, second; QString error;
    QVERIFY2(store.open(first, error), qPrintable(error));
    QVERIFY(first.isNew);
    QVERIFY(!first.id.isNull());
    const QByteArray data("identity test data");
    QVERIFY2(ProfileStore::writeSavedata(first, data, error), qPrintable(error));
    QVERIFY2(store.open(second, error), qPrintable(error));
    QVERIFY(!second.isNew);
    QCOMPARE(second.id, first.id);
    QCOMPARE(second.name, first.name);
    QCOMPARE(second.created, first.created);
    QCOMPARE(second.directory, first.directory);
}

void FoundationTest::malformedMetadata() {
    QTemporaryDir dir; ProfileStore store(dir.path()); Profile profile; QString error;
    QVERIFY(store.open(profile, error));
    QVERIFY(ProfileStore::writeSavedata(profile, QByteArray("state"), error));
    QFile file(profile.directory + "/profile.json");
    QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Truncate));
    QCOMPARE(file.write("{garbage"), qint64(8)); file.close();
    Profile reopened;
    QVERIFY(!store.open(reopened, error));
    QVERIFY(!error.isEmpty());
    QVERIFY(QFile::exists(profile.savedataPath()));
}

void FoundationTest::atomicSavedata() {
    QTemporaryDir dir; ProfileStore store(dir.path()); Profile profile; QString error;
    QVERIFY(store.open(profile, error));
    QVERIFY(ProfileStore::writeSavedata(profile, QByteArray("original"), error));
    QVERIFY(!ProfileStore::writeSavedata(profile, {}, error));
    QByteArray read;
    QVERIFY(ProfileStore::readSavedata(profile, read, error));
    QCOMPARE(read, QByteArray("original"));
}

void FoundationTest::migrations() {
    QTemporaryDir dir; Database db; QString error;
    QVERIFY2(db.open(dir.filePath("test.db"), error), qPrintable(error));
    QCOMPARE(db.schemaVersion(error), 1);
    db.close();
    QVERIFY2(db.open(dir.filePath("test.db"), error), qPrintable(error));
    QCOMPARE(db.schemaVersion(error), 1);
    db.close();
}

void FoundationTest::addressEncoding() {
    QCOMPARE(ProfileStore::encodeAddress(QByteArray::fromHex("00abff")), QStringLiteral("00ABFF"));
}

void FoundationTest::identityRestartAndStress() {
    QTemporaryDir dir; ProfileStore store(dir.path()); Profile profile; QString error;
    QVERIFY(store.open(profile, error));
    QString firstId;
    for (int i = 0; i < 100; ++i) {
        Profile loaded;
        if (i == 0) loaded = profile;
        else QVERIFY2(store.open(loaded, error), qPrintable(error));
        ToxService service(loaded, nullptr, false);
        QSignalSpy ready(&service, &ToxService::identityReady);
        QSignalSpy failure(&service, &ToxService::failure);
        service.start();
        const bool received = ready.wait(10000);
        service.stop();
        QVERIFY2(received, "Tox identity was not ready");
        QVERIFY2(failure.isEmpty(), "Tox worker reported an error");
        const QString id = ready.first().first().toString();
        QCOMPARE(id.size(), 2 * 38); // TOX_ADDRESS_SIZE: public key + nospam + checksum
        if (i == 0) firstId = id;
        else QCOMPARE(id, firstId);
        QVERIFY(!service.isRunning());
    }
    QVERIFY(QFile::exists(profile.savedataPath()));
}

QTEST_GUILESS_MAIN(FoundationTest)
#include "TestFoundation.moc"
