#ifndef SYSTEM_H
#define SYSTEM_H

#include <QObject>
#include <QQmlEngine>

#include "setting.h"

class System : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit System(QObject *parent = nullptr);

public slots:
    void restartApplication();
    void powerOff();
    void reboot();

signals:

private:
    Setting<bool> _managePower;
};

#endif // SYSTEM_H
