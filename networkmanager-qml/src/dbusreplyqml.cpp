#include "dbusreplyqml.h"

QVariant DBusReply::value() const
{
    return _value;
}

void DBusReply::setValue(const QVariant &newValue)
{
    if (_value == newValue)
        return;
    _value = newValue;
    emit valueChanged();
}

DBusReply::Status DBusReply::status() const
{
    return _status;
}

void DBusReply::refreshState()
{
    if (_reply.isError())
        this->setStatus(Failed);

    else if (!_reply.isFinished())
        this->setStatus(Pending);

    else {
        this->setStatus(Successful);

        const auto &arguments = _reply.reply().arguments();
        if (arguments.size())
            this->setValue(arguments.first());
    }
}

void DBusReply::setStatus(const Status &newStatus)
{
    if (_status == newStatus)
        return;
    _status = newStatus;
    emit statusChanged();

    if (_status != Pending)
        emit finished();
}

DBusReply::DBusReply(const QDBusPendingCall &reply, QObject *parent)
    : QObject{parent}
    , _status{Init}
    , _reply{reply}
{
    this->refreshState();

    if (_status == Pending) {
        auto watcher = new QDBusPendingCallWatcher(reply, this);
        connect(watcher, &QDBusPendingCallWatcher::finished, this, &DBusReply::refreshState);
    }
}
