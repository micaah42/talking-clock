#include "actionday.h"

#include <QLoggingCategory>

#include <orm/db.hpp>

#include "pathservice.h"

namespace {
Q_LOGGING_CATEGORY(self, "actiondays")
}

using namespace Orm;

ActionDayService::ActionDayService(QObject *parent)
    : QObject{parent}
    , _file{QString("%1/action-days.csv").arg(PathService::AppHome)}
{
    connect(&_timer, &QTimer::timeout, this, &ActionDayService::onTimeout);
    _timer.setTimerType(Qt::CoarseTimer);
    _timer.setSingleShot(true);

    this->onTimeout();
}

QList<ActionDay *> ActionDayService::actionDays(const QDate &date, QObject *parent)
{
    auto query = _model.queryBuilder();
    query->whereEq("date", date.toString("MM-dd"));
    QList<ActionDay *> actionDays = _model.get(query, this);
    qCInfo(self) << "found" << actionDays.size() << "action days for" << date;
    return actionDays;
}

void ActionDayService::onTimeout()
{
    auto oldDays = _days;

    _days = this->actionDays(QDate::currentDate(), this);
    emit daysChanged();

    for (auto day : std::as_const(oldDays))
        day->deleteLater();

    auto today = QDate::currentDate();
    auto now = QDateTime::currentDateTime();
    auto midnight = QDateTime{today.addDays(1), QTime{0, 0, 10}};

    _timer.start(now.msecsTo(midnight));
    qCInfo(self) << "action days today:" << _days.size() << "next in" << _timer.remainingTime() / (60 * 60 * 1000) << 'h';
}

ActionDay::ActionDay(QObject *parent)
    : QObject{nullptr}
{}

QDate ActionDay::date() const
{
    return m_date;
}

void ActionDay::setDate(const QDate &newDate)
{
    if (m_date == newDate)
        return;

    m_date = newDate;
    emit dateChanged();
}

QString ActionDay::name() const
{
    return m_name;
}

void ActionDay::setName(const QString &newName)
{
    if (m_name == newName)
        return;

    m_name = newName;
    emit nameChanged();
}

QString ActionDay::desc() const
{
    return m_desc;
}

void ActionDay::setDesc(const QString &newDesc)
{
    if (m_desc == newDesc)
        return;

    m_desc = newDesc;
    emit descChanged();
}

QString ActionDay::link() const
{
    return m_link;
}

void ActionDay::setLink(const QString &newLink)
{
    if (m_link == newLink)
        return;

    m_link = newLink;
    emit linkChanged();
}

const QList<ActionDay *> &ActionDayService::days() const
{
    return _days;
}

QString ActionDay::icon() const
{
    return m_icon;
}

void ActionDay::setIcon(const QString &newIcon)
{
    if (m_icon == newIcon)
        return;
    m_icon = newIcon;
    emit iconChanged();
}
