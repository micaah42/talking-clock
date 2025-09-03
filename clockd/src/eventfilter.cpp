#include "eventfilter.h"

#include <QClipboard>
#include <QDir>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QLoggingCategory>
#include <QQuickWindow>
#include <QStandardPaths>

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

void EventFilter::installToObject(QObject *object)
{
    object->installEventFilter(this);
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

    case QEvent::KeyRelease: {
        auto keyEvent = dynamic_cast<QKeyEvent *>(ev);
        if (keyEvent && keyEvent->key() == Qt::Key_F12) {
            this->makeScreenshot();
            return true;
        }
    }

    default:
        break;
    }

    return QObject::eventFilter(obj, ev);
}

void EventFilter::makeScreenshot()
{
    qCDebug(self) << __PRETTY_FUNCTION__;

    auto focusWindow = QGuiApplication::focusWindow();
    if (!focusWindow)
        return;

    auto window = qobject_cast<QQuickWindow *>(focusWindow);
    if (!window)
        return;

    auto image = window->grabWindow();
    QGuiApplication::clipboard()->setImage(image, QClipboard::Clipboard);
    QDir location{QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)};
    auto timeString = QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss");
    auto fileName = location.filePath(QString{"talking-clock-%1.jpg"}.arg(timeString));
    image.save(fileName, "PNG");

    qCInfo(self) << "saved screenshot to" << fileName;
}
