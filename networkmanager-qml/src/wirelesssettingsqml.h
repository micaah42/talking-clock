#ifndef WIRELESSSETTINGSQML_H
#define WIRELESSSETTINGSQML_H

#include <QObject>
#include <QQmlEngine>

#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/WirelessSecuritySetting>
#include <NetworkManagerQt/WirelessSetting>

namespace NM = NetworkManager;

class WirelessSetting : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")
    Q_PROPERTY(KeyMgmt keyMgmt READ keyMgmt WRITE setKeyMgmt NOTIFY keyMgmtChanged FINAL)
    Q_PROPERTY(QString ssid READ ssid WRITE setSsid NOTIFY ssidChanged FINAL)
    Q_PROPERTY(QString psk READ psk WRITE setPsk NOTIFY pskChanged FINAL)
    // Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged FINAL)
    // Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged FINAL)
public:
    enum KeyMgmt {
        Unknown = -1,
        Wep,
        Ieee8021x,
        WpaNone,
        WpaPsk,
        WpaEap,
        SAE,
        WpaEapSuiteB192,
        OWE,
    };
    Q_ENUM(KeyMgmt);

    QString ssid() const;
    void setSsid(const QString &newSsid);

    QString psk() const;
    void setPsk(const QString &newPsk);

    NM::ConnectionSettings::Ptr settings() const;

    KeyMgmt keyMgmt() const;
    void setKeyMgmt(const KeyMgmt &newKeyMgmt);

signals:
    void keyMgmtChanged();
    void ssidChanged();
    void pskChanged();

protected:
    friend class WirelessNetwork;
    explicit WirelessSetting(const NM::ConnectionSettings::Ptr &connectionSettings, QObject *parent = nullptr);

private:
    NM::ConnectionSettings::Ptr _settings;
    NM::WirelessSetting::Ptr _wirelessSetting;
    NM::WirelessSecuritySetting::Ptr _wirelessSecuritySetting;
};

#endif // WIRELESSSETTINGSQML_H
