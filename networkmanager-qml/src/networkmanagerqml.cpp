#include "networkmanagerqml.h"

#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>
#include <QLoggingCategory>
#include <unistd.h>

#include "dbusreplyqml.h"
#include "wirelessdeviceqml.h"

namespace {
Q_LOGGING_CATEGORY(self, "nmqml")
}

NetworkManagerQml::NetworkManagerQml()
{
    //_secretAgent = new NM::SecretAgent(QString{"talking-clock:%1"}.arg(getpid()));
    NM::setNetworkingEnabled(true);

    qCInfo(self) << "networking enabled:" << NM::isNetworkingEnabled();

    const auto interfaces = NM::networkInterfaces();

    for (const auto &interface : interfaces)
        if (interface->type() == NM::Device::Wifi) {
            qCInfo(self) << "use wifi interface:" << interface->interfaceName();
            auto device = interface.dynamicCast<NM::WirelessDevice>();
            this->setWirelessDevice(new WirelessDevice{device, this});
            break;
        } else
            qCWarning(self) << "failed to init interface!";

    auto notifier = NM::notifier();
    connect(notifier, &NM::Notifier::networkingEnabledChanged, this, &NetworkManagerQml::networkingEnabledChanged);
    connect(notifier, &NM::Notifier::wirelessEnabledChanged, this, &NetworkManagerQml::wirelessEnabledChanged);
}

WirelessDevice *NetworkManagerQml::wirelessDevice() const
{
    return _wirelessDevice;
}

void NetworkManagerQml::setWirelessDevice(WirelessDevice *newWirelessDevice)
{
    if (_wirelessDevice == newWirelessDevice)
        return;
    _wirelessDevice = newWirelessDevice;
    emit wirelessDeviceChanged();
}

DBusReply *NetworkManagerQml::addAndActivateConnection(Settings *settings, QObject *parent)
{
    auto reply = NM::addAndActivateConnection(settings->connectionSettings()->toMap(), "/", "/");
    return parent ? new DBusReply{reply, parent} : nullptr;
}

DBusReply *NetworkManagerQml::activateConnection(Connection *connection, QObject *parent)
{
    auto reply = NM::activateConnection(connection->connection()->path(), "/", "/");
    return parent ? new DBusReply{reply, parent} : nullptr;
}

DBusReply *NetworkManagerQml::addConnection(Settings *settings, QObject *parent)
{
    auto reply = NM::addConnection(settings->connectionSettings()->toMap());
    return parent ? new DBusReply{reply, parent} : nullptr;
}

bool NetworkManagerQml::networkingEnabled() const
{
    return NM::isNetworkingEnabled();
}

void NetworkManagerQml::setNetworkingEnabled(bool newNetworkingEnabled)
{
    if (NM::isNetworkingEnabled() == newNetworkingEnabled)
        return;

    NM::setNetworkingEnabled(newNetworkingEnabled);
}

bool NetworkManagerQml::wirelessEnabled() const
{
    return NM::isWirelessEnabled();
}

void NetworkManagerQml::setWirelessEnabled(bool newWirelessEnabled)
{
    if (NM::isWirelessEnabled() == newWirelessEnabled)
        return;

    NM::setWirelessEnabled(newWirelessEnabled);
}
