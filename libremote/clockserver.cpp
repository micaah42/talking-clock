#include "clockserver.h"

#include <QJsonDocument>
#include <QLoggingCategory>
#include <QUuid>

namespace {
Q_LOGGING_CATEGORY(self, "server");
}

ClockServer::ClockServer(QObject *parent)
    : QObject{parent}, _server("tc-remote", QWebSocketServer::NonSecureMode)
{
    connect(&_server, &QWebSocketServer::newConnection, this, &ClockServer::onNewConnection);
    bool ok = _server.listen(QHostAddress::Any, 2112);
    if (!ok) {
        qCCritical(self) << "cannot listen:" << _server.errorString();
    }
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

    _ids.remove(client);
    _clients.remove(clientId);
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
