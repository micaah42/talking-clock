#ifndef ACTIVECONNECTION_H
#define ACTIVECONNECTION_H

#include <NetworkManagerQt/ActiveConnection>
#include <QObject>
#include <QQmlEngine>

namespace NM = NetworkManager;

class ActiveConnection : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    enum State { Unknown = 0, Activating, Activated, Deactivating, Deactivated };
    Q_ENUM(State);

    explicit ActiveConnection(const NM::ActiveConnection::Ptr &activeConnection, QObject *parent = nullptr);
};

#endif // ACTIVECONNECTION_H
