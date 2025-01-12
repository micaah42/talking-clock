#include "wirelesssettingsqml.h"

#include <NetworkManagerQt/Ipv4Setting>
#include <NetworkManagerQt/Ipv6Setting>

WirelessSetting::WirelessSetting(const NetworkManager::ConnectionSettings::Ptr &connectionSettings, QObject *parent)
    : QObject{parent}
    , _settings{connectionSettings}
    , _wirelessSetting{_settings->setting(NM::Setting::Wireless).dynamicCast<NM::WirelessSetting>()}
    , _wirelessSecuritySetting{_settings->setting(NM::Setting::WirelessSecurity).dynamicCast<NM::WirelessSecuritySetting>()}
{
    _wirelessSecuritySetting->setAuthAlg(NM::WirelessSecuritySetting::Open);
    _wirelessSecuritySetting->setInitialized(true);

    _wirelessSetting->setSecurity("802-11-wireless-security");
    _wirelessSetting->setInitialized(true);
}

NM::ConnectionSettings::Ptr WirelessSetting::settings() const
{
    return _settings;
}

QString WirelessSetting::ssid() const
{
    return _wirelessSetting->ssid();
}

void WirelessSetting::setSsid(const QString &newSsid)
{
    if (_wirelessSetting->ssid() == newSsid)
        return;

    _wirelessSetting->setSsid(newSsid.toUtf8());
    emit ssidChanged();
}

QString WirelessSetting::psk() const
{
    return _wirelessSecuritySetting->psk();
}

void WirelessSetting::setPsk(const QString &newPsk)
{
    if (_wirelessSecuritySetting->psk() == newPsk)
        return;

    _wirelessSecuritySetting->setPsk(newPsk);
    emit pskChanged();
}

WirelessSetting::KeyMgmt WirelessSetting::keyMgmt() const
{
    return static_cast<KeyMgmt>(_wirelessSecuritySetting->keyMgmt());
}

void WirelessSetting::setKeyMgmt(const KeyMgmt &newKeyMgmt)
{
    auto nmKeyMgmt = static_cast<NM::WirelessSecuritySetting::KeyMgmt>(newKeyMgmt);

    if (_wirelessSecuritySetting->keyMgmt() == nmKeyMgmt)
        return;

    _wirelessSecuritySetting->setKeyMgmt(nmKeyMgmt);
    emit keyMgmtChanged();
}
