#include "timeservice.h"

#include <QLoggingCategory>

namespace {
Q_LOGGING_CATEGORY(self, "time", QtInfoMsg)
}

TimeService::TimeService(QObject *parent)
    : QObject{parent}
    , _today{QDate::currentDate()}
    , _overshoot{0}
{
    auto time = QDateTime::currentDateTime().time();
    _now = QDateTime{_today, {time.hour(), time.minute(), time.second()}};
    _nowMinutes = QDateTime{_today, {time.hour(), time.minute(), 0}};
    _now15Minutes = QDateTime{_today, {time.hour(), 15 * (time.minute() / 15), 0}};
    _nowHours = QDateTime{_today, {time.hour(), 0, 0}};

    connect(&_timer, &QTimer::timeout, this, &TimeService::onTimeout);
    _timer.setTimerType(Qt::PreciseTimer);
    _timer.setSingleShot(true);

    this->onTimeout();

    qCDebug(self) << this << _now << _today << _nowMinutes << _now15Minutes << _nowHours << _overshoot;
}

QDateTime TimeService::now() const
{
    return _now;
}

void TimeService::setNow(const QDateTime &newNow)
{
    if (_now == newNow)
        return;

    qCDebug(self) << "now" << newNow;
    _now = newNow;
    emit nowChanged();
}

QDate TimeService::today() const
{
    return _today;
}

void TimeService::setToday(const QDate &newToday)
{
    if (_today == newToday)
        return;

    qCInfo(self) << "today" << newToday;
    _today = newToday;
    emit todayChanged();
}

QDateTime TimeService::nowMinutes() const
{
    return _nowMinutes;
}

void TimeService::setNowMinutes(const QDateTime &newNowMinutes)
{
    if (_nowMinutes == newNowMinutes)
        return;

    qCInfo(self) << "nowMinutes" << newNowMinutes;
    _nowMinutes = newNowMinutes;
    emit nowMinutesChanged();
}

QDateTime TimeService::now15Minutes() const
{
    return _now15Minutes;
}

void TimeService::setNow15Minutes(const QDateTime &newNow15Minutes)
{
    if (_now15Minutes == newNow15Minutes)
        return;

    qCInfo(self) << "now15Minutes" << newNow15Minutes;
    _now15Minutes = newNow15Minutes;
    emit now15MinutesChanged();
}

QDateTime TimeService::nowHours() const
{
    return _nowHours;
}

void TimeService::setNowHours(const QDateTime &newNowHours)
{
    if (_nowHours == newNowHours)
        return;

    qCInfo(self) << "nowHours" << newNowHours;
    _nowHours = newNowHours;
    emit nowHoursChanged();
}

void TimeService::onTimeout()
{
    auto now = QDateTime::currentDateTime();
    qCDebug(self) << "onTimeout" << now;

    auto time = now.time();
    QDateTime newNow{now.date(), {time.hour(), time.minute(), time.second()}};

    if (_now == newNow) {
        _timer.start(std::min(1ll, now.msecsTo(_now.addMSecs(1000 + _overshoot))));
        return;
    }

    this->setNow(newNow);
    this->setNowMinutes(QDateTime{_today, {time.hour(), time.minute(), 0}});
    this->setNow15Minutes(QDateTime{_today, {time.hour(), 15 * (time.minute() / 15), 0}});
    this->setNowHours(QDateTime{_today, {time.hour(), 0, 0}});
    this->setToday(newNow.date());

    _timer.start(now.msecsTo(_now.addMSecs(1000 + _overshoot)));
}
