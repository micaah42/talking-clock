#ifndef WIRELESSSECURITYSETTINGQML_H
#define WIRELESSSECURITYSETTINGQML_H

#include <NetworkManagerQt/WirelessSecuritySetting>
#include <QQmlEngine>

#include "settingqml.h"

class WirelessSecuritySetting : public Setting
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")
    Q_PROPERTY(QString psk READ psk WRITE setPsk NOTIFY pskChanged FINAL)
    Q_PROPERTY(KeyMgmt keyMgmt READ keyMgmt WRITE setKeyMgmt NOTIFY keyMgmtChanged FINAL)

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

    virtual Type type() const override;

    QString psk() const;
    void setPsk(const QString &newPsk);

    KeyMgmt keyMgmt() const;
    void setKeyMgmt(const KeyMgmt &newKeyMgmt);

signals:
    void keyMgmtChanged();
    void pskChanged();

protected:
    friend class Setting;
    explicit WirelessSecuritySetting(const NM::WirelessSecuritySetting::Ptr &wirelessSecuritySetting, QObject *parent = nullptr);

private:
    NM::WirelessSecuritySetting::Ptr _wirelessSecuritySetting;
};

#endif // WIRELESSSECURITYSETTINGQML_H
