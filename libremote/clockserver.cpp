#include "clockserver.h"

#include <QFile>
#include <QJsonDocument>
#include <QLoggingCategory>
#include <QSslKey>
#include <QUuid>

namespace {
Q_LOGGING_CATEGORY(self, "server");
}

ClockServer::ClockServer(QObject *parent)
    : QObject{parent}
    , _server("clock-secure", QWebSocketServer::SecureMode)
    , _server2("clock-secure", QWebSocketServer::NonSecureMode)
{
    QFile file;
    QSslConfiguration ssl;

    // read certificate
    file.setFileName("./cert.pem");
    if (!file.open(QIODevice::ReadOnly))
        qFatal("failed to load certificate: %s", qUtf8Printable(file.errorString()));

    QSslCertificate cert{&file, QSsl::Pem};
    ssl.addCaCertificate(cert);
    file.close();

    // read key
    file.setFileName("./key.pem");
    if (!file.open(QIODevice::ReadOnly))
        qFatal("failed to load key: %s", qUtf8Printable(file.errorString()));
    QSslKey key{&file, QSsl::Rsa, QSsl::Pem};
    ssl.setPrivateKey(key);
    file.close();

    _server.setSslConfiguration(ssl);

    connect(&_server, &QWebSocketServer::newConnection, this, &ClockServer::onNewConnection);

    if (!_server.listen(QHostAddress::Any, 2112))
        qFatal("cannot listen: %s", qUtf8Printable(_server.errorString()));

    qCInfo(self) << "running, available under:" << _server.serverUrl();
}

void ClockServer::onNewConnection()
{
    auto clientId = QUuid::createUuid();
    auto client = _server.nextPendingConnection();

    if (client == nullptr || !client->isValid()) {
        qCCritical(self) << "pending connection invalid!";
    }

    connect(client, &QWebSocket::disconnected, this, &ClockServer::onDisconnection);
    connect(client, &QWebSocket::binaryMessageReceived, this, &ClockServer::onMessageReceived);
    connect(client, &QWebSocket::textMessageReceived, this, [this](const QString &msg) {
        onMessageReceived(msg.toUtf8());
    });

    _clients[clientId] = client;
    _ids[client] = clientId;

    qCInfo(self) << "new client:" << clientId << client->peerName() << client->peerAddress();
    emit clientConnected(clientId);
}

void ClockServer::onDisconnection()
{
    auto client = qobject_cast<QWebSocket *>(sender());
    Q_ASSERT(_ids.contains(client));

    auto clientId = _ids[client];
    Q_ASSERT(_clients.contains(clientId));

    _clients.remove(clientId);
    _ids.remove(client);
    client->deleteLater();
}

void ClockServer::onMessageReceived(const QByteArray &message)
{
    auto client = qobject_cast<QWebSocket *>(sender());

    Q_ASSERT(client);
    Q_ASSERT(client->isValid());
    Q_ASSERT(_ids.contains(client));
    qCInfo(self) << "received message:" << client->peerAddress() << message.size() << "bytes";
    emit messageReceived(message, _ids[client]);
}

void ClockServer::broadcastMessage(const QByteArray &message)
{
    // qCInfo(self) << "broadcasting message:" << message;
    for (auto const &client : _clients) {
        client->sendTextMessage(message);
    }
}

void ClockServer::sendObjectMessage(const QJsonObject &obj, const QUuid &clientId)
{
    auto data = QJsonDocument(obj).toJson(QJsonDocument::Compact);
    sendMessage(data, clientId);
}

void ClockServer::sendMessage(const QByteArray &message, const QUuid &clientId)
{
    _clients[clientId]->sendTextMessage(message);
}
