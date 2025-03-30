#ifndef CONNECTIONQML_H
#define CONNECTIONQML_H

#include <NetworkManagerQt/Connection>
#include <NetworkManagerQt/Utils>
#include <QQmlEngine>

namespace NM = NetworkManager;

#include "settingqml.h"

class Connection : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")
    Q_PROPERTY(Settings *settings READ settings NOTIFY settingsChanged FINAL)
    Q_PROPERTY(QString name READ name CONSTANT FINAL)

public:
    explicit Connection(const NM::Connection::Ptr &connection, QObject *parent = nullptr);

    NM::Connection::Ptr connection();
    Settings *settings() const;

    QString name() const;

public slots:
    void fetchSecrets();
    void remove();
    void save();

signals:
    void settingsChanged();

protected:
    void setSettings(Settings *newSettings);
    void onUpdated();

private:
    NM::Connection::Ptr _connection;
    Settings *_settings;
};

#endif // CONNECTIONQML_H
