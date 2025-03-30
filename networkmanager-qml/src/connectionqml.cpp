#include "connectionqml.h"

#include <QLoggingCategory>

#include "dbusreplyqml.h"

namespace {
Q_LOGGING_CATEGORY(self, "nmqml.connection")
}

Connection::Connection(const NetworkManager::Connection::Ptr &connection, QObject *parent)
    : QObject{parent}
    , _connection{connection}
    , _settings{new Settings{_connection->settings(), this}}
{
    connect(connection.get(), &QObject::destroyed, this, [this]() { this->deleteLater(); });
}

NetworkManager::Connection::Ptr Connection::connection()
{
    return _connection;
}

Settings *Connection::settings() const
{
    return _settings;
}

void Connection::remove()
{
    _connection->remove();
}

void Connection::save()
{
    _connection->save();
}

void Connection::setSettings(Settings *newSettings)
{
    if (_settings == newSettings)
        return;

    if (_settings)
        _settings->deleteLater();

    _settings = newSettings;
    emit settingsChanged();
}

void Connection::onUpdated()
{
    auto newSettings = new Settings{_connection->settings(), this};
    this->setSettings(newSettings);
}

QString Connection::name() const
{
    return _connection->name();
}

void Connection::fetchSecrets()
{
    auto pending = _connection->secrets("802-11-wireless-security.psk");
    auto reply = new DBusReply{pending, this};

    connect(reply, &DBusReply::finished, this, [reply]() {
        if (reply->status() == DBusReply::Successful)
            qCInfo(self) << "received secrets:" << reply->value();
        else
            qCWarning(self) << "failed to get secrets!";

        reply->deleteLater();
    });
}
