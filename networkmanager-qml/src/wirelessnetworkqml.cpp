#include "wirelessnetworkqml.h"

#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Setting>
#include <NetworkManagerQt/Settings>
#include <QLoggingCategory>

namespace {
Q_LOGGING_CATEGORY(self, "wirelessnetwork")
}

WirelessNetwork::WirelessNetwork(const NetworkManager::WirelessNetwork::Ptr &wirelessNetwork, QObject *parent)
    : _wirelessNetwork{wirelessNetwork}
    , _security{UnknownSecurity}

{
    connect(wirelessNetwork.get(), &NM::WirelessNetwork::destroyed, this, [this]() { this->deleteLater(); });
    connect(wirelessNetwork.get(), &NM::WirelessNetwork::signalStrengthChanged, this, &WirelessNetwork::signalStrengthChanged);
    this->refreshSecurityType();

    const auto connections = NM::listConnections();

    for (auto const &connection : connections)
        if (connection->settings()->connectionType() == NM::ConnectionSettings::Wireless) {
            auto wirelessSetting = connection->settings()->setting(NM::Setting::Wireless).dynamicCast<NM::WirelessSetting>();

            if (wirelessSetting && wirelessSetting->ssid() == this->ssid()) {
                this->setConnection(new Connection{connection, this});
                break;
            }
        }
}

void WirelessNetwork::refreshSecurityType()
{
    auto device = NM::findNetworkInterface(_wirelessNetwork->device())->as<NM::WirelessDevice>();

    if (device == nullptr) {
        qCWarning(self) << "failed to find device of network!";
        return;
    }

    auto accessPoint = _wirelessNetwork->referenceAccessPoint();
    auto newSecurity = NM::findBestWirelessSecurity( //@
        device->wirelessCapabilities(),
        accessPoint->mode() == NM::AccessPoint::ApMode,
        accessPoint->mode() == NM::AccessPoint::Adhoc,
        accessPoint->capabilities(),
        accessPoint->wpaFlags(),
        accessPoint->rsnFlags());

    this->setSecurity(static_cast<SecurityType>(newSecurity));
}

int WirelessNetwork::signalStrength() const
{
    return _wirelessNetwork->signalStrength();
}

Settings *WirelessNetwork::newSettings(QObject *parent)
{
    auto device = NM::findNetworkInterface(_wirelessNetwork->device())->as<NM::WirelessDevice>();

    if (device == nullptr) {
        qCWarning(self) << "cannot find interface!";
        return nullptr;
    }

    auto settings = NM::ConnectionSettings::Ptr::create(NM::ConnectionSettings::Wireless);
    settings->setUuid(NM::ConnectionSettings::createNewUuid());
    settings->setInterfaceName(device->interfaceName());
    settings->setId(_wirelessNetwork->ssid());

    auto wirelessSetting = settings->setting(NM::Setting::Wireless).dynamicCast<NM::WirelessSetting>();
    wirelessSetting->setSsid(_wirelessNetwork->ssid().toUtf8());

    auto wirelessSecuritySetting = settings->setting(NM::Setting::WirelessSecurity).dynamicCast<NM::WirelessSecuritySetting>();
    wirelessSecuritySetting->setKeyMgmt(NM::WirelessSecuritySetting::WpaPsk);
    qCDebug(self) << "initial setting:" << *settings;

    return new Settings{settings, parent};
}

WirelessSecuritySetting::KeyMgmt WirelessNetwork::securityTypeToKeyMgmt(SecurityType securityType)
{
    switch (securityType) {
    case StaticWep:
    case DynamicWep:
        return WirelessSecuritySetting::Wep;
    case WpaPsk:
    case Wpa2Psk:
        return WirelessSecuritySetting::WpaPsk;
    case WpaEap:
    case Wpa2Eap:
        return WirelessSecuritySetting::WpaEap;
    case SAE:
        return WirelessSecuritySetting::SAE;
    case Wpa3SuiteB192:
        return WirelessSecuritySetting::WpaEapSuiteB192;
    case OWE:
        return WirelessSecuritySetting::OWE;
    case UnknownSecurity:
    case NoneSecurity:
    case Leap:
    default:
        return WirelessSecuritySetting::Unknown;
    }
}

QString WirelessNetwork::ssid() const
{
    return _wirelessNetwork->ssid();
}

WirelessNetwork::SecurityType WirelessNetwork::security() const
{
    return _security;
}

void WirelessNetwork::setSecurity(SecurityType newSecurity)
{
    if (_security == newSecurity)
        return;

    _security = newSecurity;
    emit securityChanged();
}

void WirelessNetwork::onConnectionDestroyed()
{
    this->setConnection(nullptr);
}

Connection *WirelessNetwork::connection() const
{
    return _connection;
}

void WirelessNetwork::setConnection(Connection *newConnection)
{
    if (_connection == newConnection)
        return;

    if (_connection)
        disconnect(_connection, &Connection::destroyed, this, &WirelessNetwork::onConnectionDestroyed);

    _connection = newConnection;
    emit connectionChanged();

    if (_connection)
        connect(_connection, &Connection::destroyed, this, &WirelessNetwork::onConnectionDestroyed);
}
