#include "settingqml.h"

#include "wirelesssecuritysettingqml.h"
#include "wirelesssettingqml.h"

Settings::Settings(const NetworkManager::ConnectionSettings::Ptr &connectionSettings, QObject *parent)
    : QObject{parent}
    , _connectionSettings{connectionSettings}
{
    for (const auto &setting : _connectionSettings->settings()) {
        auto settingqml = Setting::fromNMSetting(setting, this);
        if (settingqml)
            _settings.append(settingqml);
    }
}

QList<Setting *> Settings::settings() const
{
    return _settings;
}

void Settings::setSettings(const QList<Setting *> &newSettings)
{
    if (_settings == newSettings)
        return;

    _settings = newSettings;
    emit settingsChanged();
}

NM::ConnectionSettings::Ptr Settings::connectionSettings() const
{
    return _connectionSettings;
}

void Settings::setConnectionSettings(NM::ConnectionSettings::Ptr newConnectionSettings)
{
    _connectionSettings = newConnectionSettings;
}

Setting *Settings::setting(Setting::Type type) const
{
    for (auto const setting : _settings)
        if (setting->type() == type)
            return setting;

    return nullptr;
}

Setting::Setting(QObject *parent) {}

Setting *Setting::fromNMSetting(const NetworkManager::Setting::Ptr &setting, QObject *parent)
{
    switch (setting->type()) {
    case NM::Setting::Wireless:
        return new WirelessSetting{setting.dynamicCast<NM::WirelessSetting>(), parent};
    case NM::Setting::WirelessSecurity:
        return new WirelessSecuritySetting{setting.dynamicCast<NM::WirelessSecuritySetting>(), parent};
    default:
        return nullptr;
    }
}

bool Settings::autoConnect() const
{
    return _connectionSettings->autoconnect();
}

void Settings::setAutoConnect(bool newAutoConnect)
{
    if (_connectionSettings->autoconnect() == newAutoConnect)
        return;

    _connectionSettings->setAutoconnect(newAutoConnect);
    emit autoConnectChanged();
}
