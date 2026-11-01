/* SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright © 2018-2019 by The qTox Project Contributors
 * Copyright © 2024-2026 The TokTok team.
 */

#include "src/net/bootstrapnodeupdater.h"
#include "src/persistence/paths.h"

#include <QNetworkProxy>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTemporaryDir>
#include <QFile>
#include <QSignalSpy>
#include <QtTest/QtTest>

// Needed to make this type known to Qt
Q_DECLARE_METATYPE(QList<DhtServer>)

class TestBootstrapNodesUpdater : public QObject
{
    Q_OBJECT
public:
    TestBootstrapNodesUpdater();
private slots:
    void testOnline();
    void testLocal();
    void testFailureAndRepeatedRequests();
    void testPublicEndpoint();
};

TestBootstrapNodesUpdater::TestBootstrapNodesUpdater()
{
    if (qgetenv("HOME").isEmpty()) {
        qputenv("HOME", qgetenv("TEST_TMPDIR").append("/home"));
    }

    qRegisterMetaType<QList<DhtServer>>("QList<DhtServer>");
    // Contains the builtin nodes list
    Q_INIT_RESOURCE(res);
}


namespace {
void respond(QTcpServer& server, const QByteArray& body, int status = 200)
{
    QObject::connect(&server, &QTcpServer::newConnection, &server, [&server, body, status] {
        auto* socket = server.nextPendingConnection();
        QObject::connect(socket, &QTcpSocket::readyRead, socket, [socket, body, status] {
            socket->readAll();
            socket->write("HTTP/1.1 " + QByteArray::number(status) + " Test\r\nContent-Length: "
                + QByteArray::number(body.size()) + "\r\nConnection: close\r\n\r\n" + body);
            socket->disconnectFromHost();
        });
        QObject::connect(socket, &QTcpSocket::disconnected, socket, &QObject::deleteLater);
    });
}
}
void TestBootstrapNodesUpdater::testOnline()
{
    QTemporaryDir root;
    const QNetworkProxy proxy{QNetworkProxy::ProxyType::NoProxy};
    Paths paths{Paths::Portable::NonPortable};
    paths.setPortablePath(root.path());
    BootstrapNodeUpdater updater{proxy, paths};
    QSignalSpy spy(&updater, &BootstrapNodeUpdater::availableBootstrapNodes);
    QFile fixture(":/conf/nodes.json");
    QVERIFY(fixture.open(QIODevice::ReadOnly));
    QTcpServer server;
    QVERIFY(server.listen(QHostAddress::LocalHost));
    respond(server, fixture.readAll());
    updater.requestBootstrapNodes(QUrl(QString("http://127.0.0.1:%1/nodes").arg(server.serverPort())));
    QVERIFY(spy.wait(10000));
    QCOMPARE(spy.count(), 1);
    QVERIFY(!qvariant_cast<QList<DhtServer>>(spy.at(0).at(0)).empty());
}

void TestBootstrapNodesUpdater::testFailureAndRepeatedRequests()
{
    QTemporaryDir root;
    Paths paths; paths.setPortablePath(root.path());
    BootstrapNodeUpdater updater{QNetworkProxy(QNetworkProxy::NoProxy), paths};
    QTcpServer server;
    QVERIFY(server.listen(QHostAddress::LocalHost));
    respond(server, "offline", 503);
    QSignalSpy spy(&updater, &BootstrapNodeUpdater::availableBootstrapNodes);
    const QUrl url(QString("http://127.0.0.1:%1/nodes").arg(server.serverPort()));
    for (int i = 1; i <= 3; ++i) {
        updater.requestBootstrapNodes(url);
        QTRY_COMPARE(spy.count(), i);
    }
    QVERIFY(qvariant_cast<QList<DhtServer>>(spy.last().at(0)).empty());
    QVERIFY(!updater.getBootstrapNodes().isEmpty()); // local fallback survives online failure
}

void TestBootstrapNodesUpdater::testPublicEndpoint()
{
    if (qEnvironmentVariableIntValue("PARLEY_PUBLIC_NETWORK_TESTS") != 1)
        QSKIP("External service test is opt-in; deterministic localhost HTTP and failure/fallback tests always run");
    QTemporaryDir root;
    Paths paths; paths.setPortablePath(root.path());
    BootstrapNodeUpdater updater{QNetworkProxy(QNetworkProxy::NoProxy), paths};
    QSignalSpy spy(&updater, &BootstrapNodeUpdater::availableBootstrapNodes);
    updater.requestBootstrapNodes();
    QVERIFY(spy.wait(15000));
    QVERIFY(!qvariant_cast<QList<DhtServer>>(spy.first().first()).empty());
}

void TestBootstrapNodesUpdater::testLocal()
{
    const QList<DhtServer> defaultNodes = BootstrapNodeUpdater::loadDefaultBootstrapNodes();
    QVERIFY(!defaultNodes.empty());
}

QTEST_MAIN(TestBootstrapNodesUpdater)
#include "bsu_test.moc"
