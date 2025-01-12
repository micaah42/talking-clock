#ifndef WIRELESSNETWORKQML_H
#define WIRELESSNETWORKQML_H

#include <NetworkManagerQt/Utils>
#include <NetworkManagerQt/WirelessNetwork>
#include <QQmlEngine>

#include "wirelesssettingsqml.h"

namespace NM = NetworkManager;

class WirelessNetwork : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")
    Q_PROPERTY(QString ssid READ ssid CONSTANT FINAL)
    Q_PROPERTY(SecurityType security READ security NOTIFY securityChanged FINAL)
    Q_PROPERTY(int signalStrength READ signalStrength NOTIFY signalStrengthChanged FINAL)
    Q_PROPERTY(WirelessSetting *wirelessSetting READ wirelessSetting WRITE setWirelessSetting NOTIFY wirelessSettingChanged FINAL)

public:
    enum SecurityType {
        UnknownSecurity = -1,
        NoneSecurity,
        StaticWep,
        DynamicWep,
        Leap,
        WpaPsk,
        WpaEap,
        Wpa2Psk,
        Wpa2Eap,
        SAE,
        Wpa3SuiteB192,
        OWE,
    };
    Q_ENUM(SecurityType)

    static WirelessSetting::KeyMgmt securityTypeToKeyMgmt(SecurityType securityType);

    QString ssid() const;
    int signalStrength() const;
    SecurityType security() const;

    WirelessSetting *wirelessSetting() const;
    void setWirelessSetting(WirelessSetting *newWirelessSetting);

    NM::Connection::Ptr connection() const;
    void setConnection(NM::Connection::Ptr newConnection);

public slots:
    WirelessSetting *newWirelessSettings(QObject *parent);
    //void updateSettings(WirelessSetting *newSettings);
    void refreshSecurityType();

signals:
    void wirelessSettingChanged();
    void signalStrengthChanged();
    void securityChanged();

protected:
    friend class NetworkManagerQml;
    explicit WirelessNetwork(const NM::WirelessNetwork::Ptr &wirelessNetwork, QObject *parent);
    void setSecurity(SecurityType newSecurity);

private:
    NM::WirelessNetwork::Ptr _wirelessNetwork;
    WirelessSetting *_wirelessSetting = nullptr;
    NM::Connection::Ptr _connection;
    SecurityType _security;
};

#endif
