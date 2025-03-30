#include "wirelesssettingqml.h"

#include <NetworkManagerQt/Ipv4Setting>
#include <NetworkManagerQt/Ipv6Setting>

WirelessSetting::WirelessSetting(const NetworkManager::WirelessSetting::Ptr &wirelessSetting, QObject *parent)
    : Setting{parent}
    , _wirelessSetting{wirelessSetting}
{
    _wirelessSetting->setSecurity("802-11-wireless-security");
    _wirelessSetting->setInitialized(true);
}

Setting::Type WirelessSetting::type() const
{
    return Wireless;
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

