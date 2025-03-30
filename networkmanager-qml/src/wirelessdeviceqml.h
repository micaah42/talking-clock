#ifndef WIRELESSDEVICEQML_H
#define WIRELESSDEVICEQML_H

#include <QObject>
#include <QQmlEngine>

#include <NetworkManagerQt/Connection>
#include <NetworkManagerQt/WirelessDevice>

#include "qlistmodel.h"

#include "activeconnection.h"
#include "wirelessnetworkqml.h"

namespace NM = NetworkManager;

class WirelessDevice : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")
    Q_PROPERTY(ActiveConnection *activeConnection READ activeConnection WRITE setActiveConnection NOTIFY activeConnectionChanged FINAL)
    Q_PROPERTY(WirelessNetwork *activeNetwork READ activeNetwork NOTIFY activeNetworkChanged FINAL)
    Q_PROPERTY(QListModelBase *wirelessNetworks READ wirelessNetworks CONSTANT FINAL)

public:
    explicit WirelessDevice(NM::WirelessDevice::Ptr &wirelessDevice, QObject *parent = nullptr);
    QListModel<WirelessNetwork *> *wirelessNetworks();
    WirelessNetwork *activeNetwork() const;
    ActiveConnection *activeConnection() const;

public slots:
    void scan();

signals:
    void activeNetworkChanged();
    void activeConnectionChanged();

protected:
    NM::Connection::Ptr findConnection(const QString &ssid);
    WirelessNetwork *findNetwork(const QString &ssid);
    void setActiveConnection(ActiveConnection *newActiveConnection);
    void setActiveNetwork(WirelessNetwork *newActiveNetwork);

    void onConnectionAdded(const QString &connecion);
    void onNetworkAppeared(const QString &ssid);
    void onActiveAccessPointChanged(const QString &path);
    void onActiveConnectionChanged();

private:
    NM::WirelessDevice::Ptr _wirelessDevice;
    QListModel<WirelessNetwork *> _wirelessNetworks;
    WirelessNetwork *m_activeNetwork = nullptr;
    ActiveConnection *m_activeConnection = nullptr;
};

#endif // NETWORKMANAGERQML_H
