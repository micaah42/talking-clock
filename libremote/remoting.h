#ifndef REMOTING_H
#define REMOTING_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMap>
#include <QMetaMethod>
#include <QMetaProperty>
#include <QObject>
#include <QUuid>

#include "clockserver.h"
#include "serializer.h"

class Remoting : public QObject
{
    Q_OBJECT
public:
    static QByteArray pong();

    explicit Remoting(QObject *parent = nullptr);

    // (de)register c++ objects and its children to remoting
    void registerObject(const QString &name, QObject *object);
    void deregisterObject(const QString &name);

    // access/subsribe registered object's properties
    Q_INVOKABLE QJsonValue value(const QString &key);
    Q_INVOKABLE void subscribe(const QString &key);

    Q_INVOKABLE QJsonArray receivers();
    Q_INVOKABLE QJsonArray methods(const QString &receiver);

private slots:
    void onNotifySignal();
    void onModelChanged(const QModelIndex &from, const QModelIndex &to, QVector<int> roles);
    void onModelRowsRemoved(const QModelIndex &parent, const int first, const int last);

    void onMessageReceived(const QByteArray &message, const QUuid &clientId);
    QJsonObject processCommand(const QJsonObject &cmd);

private:
    QMap<QString, QObject *> _objects;
    QMap<QObject *, QMap<QString, QMetaMethod>> _methodsOld;
    QMap<QObject *, QMap<QString, QList<QMetaProperty>>> _propertiesOld;
    QMap<QObject *, QString> _modelKeys;

    QMap<QString, QPair<QObject *, QMetaMethod>> _methods;
    QMap<QString, QPair<QObject *, QMetaProperty>> _properties;

    QMap<QPair<QObject *, int>, QString> _notifierCache;

    ClockServer _server;
};

#endif // REMOTING_H
