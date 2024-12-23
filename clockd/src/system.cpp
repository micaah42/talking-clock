#include "system.h"

#include "QGuiApplication"
#include "QProcess"
#include "QTimer"

#define DELAY_TIME 1000

#ifdef Q_PROCESSOR_X86_64
#define MANAGE_POWER false
#else
#define MANAGE_POWER true
#endif

System::System(QObject *parent)
    : QObject{parent}
    , _managePower{"System/ManagePower", MANAGE_POWER}
{}

void System::powerOff()
{
    if (_managePower) {
        QTimer::singleShot(DELAY_TIME, []() {
            QProcess::startDetached("/usr/sbin/poweroff");
            QGuiApplication::quit();
        });
    }
}

void System::reboot()
{
    if (_managePower) {
        QTimer::singleShot(DELAY_TIME, []() {
            QProcess::startDetached("/usr/sbin/reboot");
            QGuiApplication::quit();
        });
    }
}
