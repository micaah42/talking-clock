#ifndef SETTINGQML_H
#define SETTINGQML_H

#include <NetworkManagerQt/Settings>
#include <QQmlEngine>

namespace NM = NetworkManager;

class Setting : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")
    Q_PROPERTY(Type type READ type CONSTANT FINAL)

public:
    enum Type {
        General,
        Wireless,
        WirelessSecurity,
    };
    Q_ENUM(Type);

    static Setting *fromNMSetting(const NM::Setting::Ptr &setting, QObject *parent);
    virtual Type type() const = 0;

protected:
    explicit Setting(QObject *parent);
};

class Settings : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")
    Q_PROPERTY(bool autoConnect READ autoConnect WRITE setAutoConnect NOTIFY autoConnectChanged FINAL)
    Q_PROPERTY(QList<Setting *> settings READ settings WRITE setSettings NOTIFY settingsChanged FINAL)

public:
    explicit Settings(const NM::ConnectionSettings::Ptr &connectionSetting, QObject *parent = nullptr);

    QList<Setting *> settings() const;

    NM::ConnectionSettings::Ptr connectionSettings() const;
    void setConnectionSettings(NM::ConnectionSettings::Ptr newConnectionSettings);

    bool autoConnect() const;
    void setAutoConnect(bool newAutoConnect);

public slots:
    Setting *setting(Setting::Type type) const;

signals:
    void settingsChanged();

    void autoConnectChanged();

protected:
    void setSettings(const QList<Setting *> &newSettings);

private:
    NM::ConnectionSettings::Ptr _connectionSettings;
    QList<Setting *> _settings;
};

#endif // SETTINGQML_H
