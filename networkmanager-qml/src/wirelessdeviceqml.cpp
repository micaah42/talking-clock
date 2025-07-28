#include "wirelessdeviceqml.h"

#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>
#include <QLoggingCategory>

#include "wirelessnetworkqml.h"

namespace {
Q_LOGGING_CATEGORY(self, "nmqml.wireless")
}

WirelessDevice::WirelessDevice(NetworkManager::WirelessDevice::Ptr &wirelessDevice, QObject *parent)
    : QObject{parent}
    , _wirelessDevice{wirelessDevice}
{
    connect(_wirelessDevice.get(), &NM::WirelessDevice::activeConnectionChanged, this, &WirelessDevice::onActiveConnectionChanged);
    connect(_wirelessDevice.get(), &NM::WirelessDevice::activeAccessPointChanged, this, &WirelessDevice::onActiveAccessPointChanged);
    connect(_wirelessDevice.get(), &NM::WirelessDevice::networkAppeared, this, &WirelessDevice::onNetworkAppeared);
    connect(NM::settingsNotifier(), &NM::SettingsNotifier::connectionAdded, this, &WirelessDevice::onConnectionAdded);

    const auto networks = _wirelessDevice->networks();
    for (auto const &network : networks) {
        _wirelessNetworks.append(new WirelessNetwork{network, this});
        qCInfo(self) << "found network:" << network->ssid();
    }

    this->onActiveAccessPointChanged("");
}

WirelessNetwork *WirelessDevice::activeNetwork() const
{
    return m_activeNetwork;
}

void WirelessDevice::setActiveNetwork(WirelessNetwork *newActiveNetwork)
{
    if (m_activeNetwork == newActiveNetwork)
        return;

    m_activeNetwork = newActiveNetwork;
    emit activeNetworkChanged();
}

void WirelessDevice::onNetworkAppeared(const QString &ssid)
{
    auto network = _wirelessDevice->findNetwork(ssid);

    if (network)
        _wirelessNetworks.append(new WirelessNetwork{network, this});
}

void WirelessDevice::onActiveAccessPointChanged(const QString &path)
{
    auto activeAccessPoint = _wirelessDevice->activeAccessPoint();

    if (activeAccessPoint == nullptr) {
        this->setActiveNetwork(nullptr);
        return;
    }

    auto network = findNetwork(activeAccessPoint->ssid());
    this->setActiveNetwork(network);
}

void WirelessDevice::onActiveConnectionChanged()
{
    this->setActiveConnection(new ActiveConnection{_wirelessDevice->activeConnection(), this});
}

ListModel<WirelessNetwork *> *WirelessDevice::wirelessNetworks()
{
    return &_wirelessNetworks;
}

void WirelessDevice::scan()
{
    _wirelessDevice->requestScan();
}

WirelessNetwork *WirelessDevice::findNetwork(const QString &ssid)
{
    for (const auto &network : _wirelessNetworks)
        if (network->ssid() == ssid)
            return network;

    return nullptr;
}

NM::Connection::Ptr WirelessDevice::findConnection(const QString &ssid)
{
    const auto connections = NM::listConnections();

    for (const auto &connection : connections) {
        if (connection->settings()->connectionType() != NM::ConnectionSettings::Wireless)
            continue;
        if (connection->settings()->interfaceName() != _wirelessDevice->interfaceName())
            continue;

        auto settings = connection->settings()->setting(NM::Setting::Wireless).dynamicCast<NM::WirelessSetting>();
        if (settings->ssid() == ssid)
            return connection;
    }
    return nullptr;
}

void WirelessDevice::onConnectionAdded(const QString &path)
{
    auto connection = NM::Connection::Ptr::create(path);

    if (connection->settings()->connectionType() != NM::ConnectionSettings::Wireless)
        return;
    if (connection->settings()->interfaceName() != _wirelessDevice->interfaceName())
        return;

    auto settings = connection->settings()->setting(NM::Setting::Wireless).dynamicCast<NM::WirelessSetting>();
    auto network = findNetwork(settings->ssid());

    if (network)
        network->setConnection(new Connection{connection, network});
}

ActiveConnection *WirelessDevice::activeConnection() const
{
    return m_activeConnection;
}

void WirelessDevice::setActiveConnection(ActiveConnection *newActiveConnection)
{
    if (m_activeConnection == newActiveConnection)
        return;

    if (m_activeConnection)
        m_activeConnection->deleteLater();

    m_activeConnection = newActiveConnection;
    emit activeConnectionChanged();
}
