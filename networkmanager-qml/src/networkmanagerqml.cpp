#include "networkmanagerqml.h"

#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>
#include <QLoggingCategory>

#include "wirelessnetworkqml.h"

namespace {
Q_LOGGING_CATEGORY(self, "wireless")
}

NetworkManagerQml::NetworkManagerQml()
{
    NM::setNetworkingEnabled(true);
    qCInfo(self) << "networking enabled:" << NM::isNetworkingEnabled();

    for (const auto &connection : NM::listConnections()) {
        if (connection->settings()->connectionType() != NM::ConnectionSettings::Wireless)
            continue;

        auto settings = connection->settings()->setting(NM::Setting::Wireless).dynamicCast<NM::WirelessSetting>();
        _connections.insert(settings->ssid(), connection);
    }

    const auto interfaces = NM::networkInterfaces();

    for (const auto &interface : interfaces)
        if (interface->type() == NM::Device::Wifi) {
            qCInfo(self) << "use wifi interface:" << interface->interfaceName();
            _wirelessDevice.reset(interface->as<NM::WirelessDevice>());
            break;
        }

    connect(_wirelessDevice.get(), &NM::WirelessDevice::networkDisappeared, this, &NetworkManagerQml::onNetworkDisappeared);
    connect(_wirelessDevice.get(), &NM::WirelessDevice::networkAppeared, this, &NetworkManagerQml::onNetworkAppeared);

    if (_wirelessDevice) {
        for (auto const &network : _wirelessDevice->networks()) {
            _wirelessNetworks.append(new WirelessNetwork{network, this});
            qCInfo(self) << "found network:" << network->ssid();
        }
    }

    else
        qCWarning(self) << "failed to init interface!";
}

void NetworkManagerQml::onNetworkDisappeared(const QString &ssid)
{
    auto networks = _wirelessDevice->networks();
    for (int i = 0; i < networks.size(); i++)
        if (networks[i]->ssid() == ssid)
            _wirelessNetworks.removeAt(i);
}

void NetworkManagerQml::onNetworkAppeared(const QString &ssid)
{
    NM::WirelessNetwork::Ptr newNetwork = _wirelessDevice->findNetwork(ssid);

    if (newNetwork.isNull()) {
        qWarning() << "failed to find appeared network:" << ssid;
        return;
    }

    for (int i = 0; i < _wirelessNetworks.size() - 1; i++) {
        if (_wirelessNetworks.at(i + 1)->signalStrength() < newNetwork->signalStrength()) {
            auto network = new WirelessNetwork{newNetwork, this};
            _wirelessNetworks.insert(i, network);
            break;
        }
    }
}

void NetworkManagerQml::onConnectionRemoved(const QString &path)
{
    for (int i = 0; i < _wirelessNetworks.size(); i++) {
        auto network = _wirelessNetworks.at(i);

        if (network->connection()->path() == path) {
            network->setWirelessSetting(nullptr);
            network->setConnection(nullptr);
        }
    }
}

void NetworkManagerQml::onConnectionAdded(const QString &path)
{
    auto connection = NM::Connection::Ptr::create(path);
    if (connection->settings()->connectionType() != NM::ConnectionSettings::Wireless)
        return;

    auto settings = connection->settings()->setting(NM::Setting::Wireless).dynamicCast<NM::WirelessSetting>();
    _connections.insert(settings->ssid(), connection);
}

QListModel<WirelessNetwork *> *NetworkManagerQml::wirelessNetworks()
{
    return &_wirelessNetworks;
}

void NetworkManagerQml::scan()
{
    _wirelessDevice->requestScan();
}

