#ifndef NETWORKMANAGERQML_H
#define NETWORKMANAGERQML_H

#include <QObject>
#include <QQmlEngine>

#include <NetworkManagerQt/Connection>
#include <NetworkManagerQt/SecretAgent>
#include <NetworkManagerQt/WirelessDevice>

#include "settingqml.h"

class WirelessDevice;
Q_DECLARE_OPAQUE_POINTER(WirelessDevice *);

//class Settings;
//Q_DECLARE_OPAQUE_POINTER(Settings *);

class DBusReply;
Q_DECLARE_OPAQUE_POINTER(DBusReply *);

class Connection;
Q_DECLARE_OPAQUE_POINTER(Connection *);

namespace NM = NetworkManager;

class NetworkManagerQml : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(WirelessDevice *wirelessDevice READ wirelessDevice WRITE setWirelessDevice NOTIFY wirelessDeviceChanged FINAL)
    Q_PROPERTY(bool networkingEnabled READ networkingEnabled WRITE setNetworkingEnabled NOTIFY networkingEnabledChanged FINAL)
    Q_PROPERTY(bool wirelessEnabled READ wirelessEnabled WRITE setWirelessEnabled NOTIFY wirelessEnabledChanged FINAL)
    Q_PROPERTY(NM::Connectivity connectivity READ connectivity NOTIFY connectivityChanged FINAL)

public:
    explicit NetworkManagerQml();

    WirelessDevice *wirelessDevice() const;
    void setWirelessDevice(WirelessDevice *newWirelessDevice);

    bool networkingEnabled() const;
    void setNetworkingEnabled(bool newNetworkingEnabled);

    bool wirelessEnabled() const;
    void setWirelessEnabled(bool newWirelessEnabled);

    NM::Connectivity connectivity() const;

public slots:
    DBusReply *addAndActivateConnection(Settings *settings, QObject *parent = nullptr);
    DBusReply *activateConnection(Connection *connection, QObject *parent = nullptr);
    DBusReply *addConnection(Settings *settings, QObject *parent = nullptr);

signals:
    void networkingEnabledChanged();
    void wirelessEnabledChanged();
    void wirelessDeviceChanged();
    void connectivityChanged();

private:
    NM::SecretAgent *_secretAgent;
    WirelessDevice *_wirelessDevice;
};

#endif // NETWORKMANAGERQML_H
