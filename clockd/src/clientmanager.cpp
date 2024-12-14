#include "clientmanager.h"

#include <QWebSocket>

#include "websocketserver.h"

ClientService::ClientService(WebSocketServer &server, QObject *parent)
    : QObject{parent}
    , _server{server}
    , m_activeClientCount{0}
{
    connect(&_server, &WebSocketServer::clientConnected, this, &ClientService::onClientConnected);
}

Client::Client(QObject *parent)
    : QObject{parent}
    , m_online{false}
    , m_pingable{false}
{}

QString Client::id() const
{
    return m_id;
}

void Client::setId(const QString &newId)
{
    if (m_id == newId)
        return;

    m_id = newId;
    emit idChanged();
}

QString Client::ip() const
{
    return m_ip;
}

void Client::setIp(const QString &newIp)
{
    if (m_ip == newIp)
        return;

    m_ip = newIp;
    emit ipChanged();
}

QDateTime Client::lastOnline() const
{
    return m_lastOnline;
}

void Client::setLastOnline(const QDateTime &newLastOnline)
{
    if (m_lastOnline == newLastOnline)
        return;
    m_lastOnline = newLastOnline;
    emit lastOnlineChanged();
}

bool Client::pingable() const
{
    return m_pingable;
}

void Client::setPingable(bool newPingable)
{
    if (m_pingable == newPingable)
        return;
    m_pingable = newPingable;
    emit pingableChanged();
}

bool Client::online() const
{
    return m_online;
}

void Client::setOnline(bool newOnline)
{
    if (m_online == newOnline)
        return;
    m_online = newOnline;
    emit onlineChanged();
}

QListModel<Client *> *ClientService::clients()
{
    return &_clients;
}

Client *ClientService::findById(const QString &id)
{
    auto it = std::find_if(_clients.cbegin(), _clients.cend(), [&id](const Client *client) { return id == client->ip(); });
    return it == _clients.cend() ? nullptr : *it;
}

void ClientService::onClientConnected(QWebSocket *socket)
{
    const auto peerAddress = socket->peerAddress().toString();
    auto client = findById(peerAddress);

    if (client == nullptr) {
        client = new Client(this);
        client->setIp(peerAddress);
    }

    client->setLastOnline(QDateTime::currentDateTime());
    client->setPingable(true);
    client->setOnline(true);

    m_activeClientCount += 1;
    emit activeClientCountChanged();

    connect(socket, &QWebSocket::disconnected, this, [this, client]() {
        client->setLastOnline(QDateTime::currentDateTime());
        client->setOnline(false);

        m_activeClientCount -= 1;
        emit activeClientCountChanged();
    });

    _clients.append(client);
}

int ClientService::activeClientCount() const
{
    return m_activeClientCount;
}

void ClientService::setActiveClientCount(int newActiveClientCount)
{
    if (m_activeClientCount == newActiveClientCount)
        return;

    m_activeClientCount = newActiveClientCount;
    emit activeClientCountChanged();
}
