#ifndef CLOCKSERVER_H
#define CLOCKSERVER_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>

#include <qobjectregistry.h>

class ClockServer : public QObject
{
    Q_OBJECT
public:
    explicit ClockServer(QObjectRegistry &registry, QObject *parent = nullptr);

signals:

private slots:
    void onNewConnection();

private:
    QObjectRegistry &_registry;

    QWebSocketServer _server;
    QList<QWebSocket *> _sockets;
};

#endif // CLOCKSERVER_H
