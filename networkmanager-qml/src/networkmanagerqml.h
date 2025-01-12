#ifndef NETWORKMANAGERQML_H
#define NETWORKMANAGERQML_H

#include <QObject>
#include <QQmlEngine>

#include <NetworkManagerQt/Connection>
#include <NetworkManagerQt/WirelessDevice>

#include "qlistmodel.h"

#include "wirelesssettingsqml.h"

namespace NM = NetworkManager;

class WirelessNetwork;
class WirelessSetting;
class QPainter;

class NetworkManagerQml : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(QListModelBase *wirelessNetworks READ wirelessNetworks CONSTANT FINAL)

public:
    explicit NetworkManagerQml();
    QListModel<WirelessNetwork *> *wirelessNetworks();

public slots:
    void scan();

protected:
    void onNetworkDisappeared(const QString &ssid);
    void onNetworkAppeared(const QString &ssid);

    void onConnectionRemoved(const QString &path);
    void onConnectionAdded(const QString &path);

private:
    NM::WirelessDevice::Ptr _wirelessDevice;
    QListModel<WirelessNetwork *> _wirelessNetworks;
    QMap<QString, NM::Connection::Ptr> _connections;
};

#endif // NETWORKMANAGERQML_H
