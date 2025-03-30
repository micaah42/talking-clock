#include "wirelesssecuritysettingqml.h"

Setting::Type WirelessSecuritySetting::type() const
{
    return WirelessSecurity;
}

WirelessSecuritySetting::WirelessSecuritySetting(const NetworkManager::WirelessSecuritySetting::Ptr &wirelessSecuritySetting, QObject *parent)
    : Setting{parent}
    , _wirelessSecuritySetting{wirelessSecuritySetting}

{
    _wirelessSecuritySetting->setAuthAlg(NM::WirelessSecuritySetting::Open);
    _wirelessSecuritySetting->setInitialized(true);
}

QString WirelessSecuritySetting::psk() const
{
    return _wirelessSecuritySetting->psk();
}

void WirelessSecuritySetting::setPsk(const QString &newPsk)
{
    if (_wirelessSecuritySetting->psk() == newPsk)
        return;

    _wirelessSecuritySetting->setPsk(newPsk);
    emit pskChanged();
}

WirelessSecuritySetting::KeyMgmt WirelessSecuritySetting::keyMgmt() const
{
    return static_cast<KeyMgmt>(_wirelessSecuritySetting->keyMgmt());
}

void WirelessSecuritySetting::setKeyMgmt(const KeyMgmt &newKeyMgmt)
{
    auto nmKeyMgmt = static_cast<NM::WirelessSecuritySetting::KeyMgmt>(newKeyMgmt);

    if (_wirelessSecuritySetting->keyMgmt() == nmKeyMgmt)
        return;

    _wirelessSecuritySetting->setKeyMgmt(nmKeyMgmt);
    emit keyMgmtChanged();
}
