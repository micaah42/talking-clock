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
    connect(wirelessNetwork.get(), &NM::WirelessNetwork::signalStrengthChanged, this, &WirelessNetwork::signalStrengthChanged);
    this->refreshSecurityType();

    for (auto const &connection : NM::listConnections()) {
        auto settings = connection->settings();
        if (settings.isNull() || settings->connectionType() != NM::ConnectionSettings::Wireless)
            continue;

        auto wirelessSetting = settings->setting(NM::Setting::Wireless).dynamicCast<NM::WirelessSetting>();
        if (wirelessSetting.isNull())
            continue;

        if (wirelessSetting->ssid() != _wirelessNetwork->ssid())
            continue;

        _connection = connection;
        this->setWirelessSetting(new WirelessSetting{settings, this});
        qCInfo(self) << "network has connection:" << *settings;
        break;
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

WirelessSetting::KeyMgmt WirelessNetwork::securityTypeToKeyMgmt(SecurityType securityType)
{
    switch (securityType) {
    case StaticWep:
    case DynamicWep:
        return WirelessSetting::Wep;
    case WpaPsk:
    case Wpa2Psk:
        return WirelessSetting::WpaPsk;
    case WpaEap:
    case Wpa2Eap:
        return WirelessSetting::WpaEap;
    case SAE:
        return WirelessSetting::SAE;
    case Wpa3SuiteB192:
        return WirelessSetting::WpaEapSuiteB192;
    case OWE:
        return WirelessSetting::OWE;
    case UnknownSecurity:
    case NoneSecurity:
    case Leap:
        return WirelessSetting::Unknown;
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

NM::Connection::Ptr WirelessNetwork::connection() const
{
    return _connection;
}

void WirelessNetwork::setConnection(NM::Connection::Ptr newConnection)
{
    _connection = newConnection;
}

WirelessSetting *WirelessNetwork::wirelessSetting() const
{
    return _wirelessSetting;
}

WirelessSetting *WirelessNetwork::newWirelessSettings(QObject *parent)
{
    if (_wirelessSetting) {
        auto settings = NM::ConnectionSettings::Ptr::create(_wirelessSetting->settings()->toMap());
        return new WirelessSetting{settings, this};
    }

    auto device = NM::findNetworkInterface(_wirelessNetwork->device())->as<NM::WirelessDevice>();

    if (device == nullptr) {
        qCWarning(self) << "cannot find interface!";
        return nullptr;
    }

    auto settings = NM::ConnectionSettings::Ptr::create(NM::ConnectionSettings::Wireless);
    settings->setUuid(NM::ConnectionSettings::createNewUuid());
    settings->setInterfaceName(device->interfaceName());
    settings->setId(_wirelessNetwork->ssid());

    auto newSettings = new WirelessSetting{settings, parent};
    newSettings->setSsid(_wirelessNetwork->ssid().toUtf8());
    newSettings->setKeyMgmt(WirelessSetting::WpaPsk);
    qCDebug(self) << "initial setting:" << *settings;

    return newSettings;
}

void WirelessNetwork::setWirelessSetting(WirelessSetting *newWirelessSetting)
{
    if (_wirelessSetting == newWirelessSetting)
        return;

    if (_connection) {
        auto pending = _connection->update(newWirelessSetting->settings()->toMap());
        auto watcher = new QDBusPendingCallWatcher{pending};
        connect(watcher, &QDBusPendingCallWatcher::finished, this, [this, watcher, pending]() {
            if (pending.isError() || !pending.isFinished())
                qCWarning(self) << "failed to update connection!";
            watcher->deleteLater();
        });
    }

    else {
        qCDebug(self) << *newWirelessSetting->settings();
        auto pending = NM::addConnection(newWirelessSetting->settings()->toMap());
        auto watcher = new QDBusPendingCallWatcher{pending};
        connect(watcher, &QDBusPendingCallWatcher::finished, this, [this, watcher, pending]() {
            if (pending.isError() || !pending.isFinished())
                qCWarning(self) << "failed to add connection!";
            watcher->deleteLater();
        });
    }
}
