#include "clockserver.h"

#include <QLoggingCategory>

#include <qopenremoteadapter.h>

namespace {
Q_LOGGING_CATEGORY(self, "server")
}

ClockServer::ClockServer(QObjectRegistry &registry, QObject *parent)
    : QObject{parent}
    , _registry{registry}
    , _server{"talking-clock", QWebSocketServer::NonSecureMode}
{
    connect(&_server, &QWebSocketServer::newConnection, this, &ClockServer::onNewConnection);

    if (!_server.listen(QHostAddress{"127.0.0.1"}, 21120))
        qCCritical(self) << "failed to start websocket server:" << _server.errorString();
}

void ClockServer::onNewConnection()
{
    if (!_server.hasPendingConnections())
        return;

    auto socket = _server.nextPendingConnection();
    auto adapter = new QOpenRemoteAdapter{_registry, socket};
    qCInfo(self) << "client connected" << socket;

    connect(socket, &QWebSocket::textMessageReceived, adapter, [adapter](const QString &message) { adapter->handleMessage(message.toUtf8()); });
    connect(adapter, &QOpenRemoteAdapter::sendMessage, socket, [socket](const QByteArray &message) { socket->sendTextMessage(message); });

    connect(socket, &QWebSocket::disconnected, this, [socket]() {
        qCInfo(self) << "client disconnected:" << socket;
        socket->deleteLater();
    });
}
