#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QDateTime>
#include <QObject>
#include <QQmlEngine>

#include "listmodel.h"

class WebSocketServer;
class QWebSocket;

class Client : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged FINAL)
    Q_PROPERTY(QString ip READ ip WRITE setIp NOTIFY ipChanged FINAL)
    Q_PROPERTY(QDateTime lastOnline READ lastOnline WRITE setLastOnline NOTIFY lastOnlineChanged FINAL)
    Q_PROPERTY(bool pingable READ pingable NOTIFY pingableChanged FINAL)
    Q_PROPERTY(bool online READ online NOTIFY onlineChanged FINAL)

public:
    Q_INVOKABLE explicit Client(QObject *parent = nullptr);

    QString id() const;
    void setId(const QString &newId);

    QString ip() const;
    void setIp(const QString &newIp);

    QDateTime lastOnline() const;
    void setLastOnline(const QDateTime &newLastOnline);

    bool pingable() const;
    bool online() const;

signals:
    void idChanged();
    void ipChanged();
    void lastOnlineChanged();
    void pingableChanged();
    void onlineChanged();

protected:
    void setPingable(bool newPingable);
    void setOnline(bool newOnline);
    friend class ClientService;

private:
    QString m_id;
    QString m_ip;
    QDateTime m_lastOnline;
    bool m_pingable;
    bool m_online;
};

class ClientService : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(int activeClientCount READ activeClientCount NOTIFY activeClientCountChanged FINAL)
    Q_PROPERTY(ListModelBase *clients READ clients CONSTANT FINAL)

public:
    explicit ClientService(WebSocketServer &server, QObject *parent = nullptr);
    ListModel<Client *> *clients();
    int activeClientCount() const;

public slots:
    Client *findById(const QString &id);

signals:
    void activeClientCountChanged();

protected:
    void setActiveClientCount(int newActiveClientCount);
    void onClientConnected(QWebSocket *socket);

private slots:

private:
    WebSocketServer &_server;
    ListModel<Client *> _clients;

    int m_activeClientCount;
};

#endif // CLIENTMANAGER_H
