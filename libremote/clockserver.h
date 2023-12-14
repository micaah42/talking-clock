#ifndef CLOCKSERVER_H
#define CLOCKSERVER_H

#include <QObject>
#include <QUuid>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>

class ClockServer : public QObject
{
    Q_OBJECT
public:
    explicit ClockServer(QObject *parent = nullptr);

signals:
    // message received
    void messageReceived(const QByteArray &message, const QUuid &client);

    // connections
    void clientConnected(const QUuid &clientId);
    void clientDisconnected(const QUuid &clientId);

public slots:
    void broadcastMessage(const QByteArray &message);
    void sendMessage(const QByteArray &message, const QUuid &clientId);
    void sendObjectMessage(const QJsonObject &obj, const QUuid &clientId);

private slots:

    void onNewConnection();
    void onDisconnection();
    void onMessageReceived(const QByteArray &message);

private:
    QMap<QWebSocket *, QUuid> _ids;
    QMap<QUuid, QWebSocket *> _clients;

    QWebSocketServer _server;
    QWebSocketServer _server2;
};

#endif // CLOCKSERVER_H
