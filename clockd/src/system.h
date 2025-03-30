#ifndef SYSTEM_H
#define SYSTEM_H

#include <QObject>

#include "setting.h"

class System : public QObject
{
    Q_OBJECT
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
