#ifndef WIRELESSSETTINGQML_H
#define WIRELESSSETTINGQML_H

#include <QQmlEngine>

#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/WirelessSetting>

#include "settingqml.h"

namespace NM = NetworkManager;

class WirelessSetting : public Setting
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")
    Q_PROPERTY(QString ssid READ ssid WRITE setSsid NOTIFY ssidChanged FINAL)

public:
    virtual Type type() const override;

    NM::ConnectionSettings::Ptr settings() const;

    QString ssid() const;
    void setSsid(const QString &newSsid);

signals:
    void ssidChanged();

protected:
    friend class Setting;
    explicit WirelessSetting(const NM::WirelessSetting::Ptr &wirelessSetting, QObject *parent = nullptr);

private:
    NM::WirelessSetting::Ptr _wirelessSetting;
};

#endif // WIRELESSSETTINGQML_H
