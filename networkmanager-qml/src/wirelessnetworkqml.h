#ifndef WIRELESSNETWORKQML_H
#define WIRELESSNETWORKQML_H

#include <NetworkManagerQt/Utils>
#include <NetworkManagerQt/WirelessNetwork>
#include <QQmlEngine>

#include "connectionqml.h"
#include "wirelesssecuritysettingqml.h"

namespace NM = NetworkManager;

class WirelessNetwork : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")
    Q_PROPERTY(QString ssid READ ssid CONSTANT FINAL)
    Q_PROPERTY(SecurityType security READ security NOTIFY securityChanged FINAL)
    Q_PROPERTY(int signalStrength READ signalStrength NOTIFY signalStrengthChanged FINAL)
    Q_PROPERTY(Connection *connection READ connection WRITE setConnection NOTIFY connectionChanged FINAL)

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

    static WirelessSecuritySetting::KeyMgmt securityTypeToKeyMgmt(SecurityType securityType);

    QString ssid() const;
    int signalStrength() const;
    SecurityType security() const;

    Connection *connection() const;
    void setConnection(Connection *newConnection);

public slots:
    Settings *newSettings(QObject *parent);
    void refreshSecurityType();

signals:
    void signalStrengthChanged();
    void securityChanged();
    void connectionChanged();

protected:
    friend class WirelessDevice;
    explicit WirelessNetwork(const NM::WirelessNetwork::Ptr &wirelessNetwork, QObject *parent);
    void setSecurity(SecurityType newSecurity);

    void onConnectionDestroyed();

private:
    NM::WirelessNetwork::Ptr _wirelessNetwork;
    Connection *_connection = nullptr;
    SecurityType _security;
};

#endif
