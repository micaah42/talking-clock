#include "eventfilter.h"

#include <QLoggingCategory>

namespace {
Q_LOGGING_CATEGORY(self, "event", QtInfoMsg)
}

EventFilter::EventFilter(QObject *parent)
    : QObject{parent}
{
    connect(&_inactivityTimer, &QTimer::timeout, this, &EventFilter::userInactive);
    _inactivityTimer.setInterval(std::chrono::seconds{60});
    _inactivityTimer.setSingleShot(true);
    _inactivityTimer.start();
}

bool EventFilter::eventFilter(QObject *obj, QEvent *ev)
{
    switch (ev->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchEnd:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
        qCDebug(self) << "user event:" << ev->type();
        _inactivityTimer.start();

    default:
        break;
    }

    return QObject::eventFilter(obj, ev);
}
