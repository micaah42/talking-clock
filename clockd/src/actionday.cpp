#include "actionday.h"

#include <QLoggingCategory>

#include "pathservice.h"

namespace {
Q_LOGGING_CATEGORY(self, "actiondays")
}

ActionDayManager::ActionDayManager(QObject *parent)
    : QObject{parent}
    , _file{QString("%1/action-days.csv").arg(PathService::AppHome)}
{}

QList<ActionDay *> ActionDayManager::actionDays(const QDate &date, QObject *parent)
{
    if (!_file.open(QIODevice::ReadOnly)) {
        qCCritical(self) << "failed to open action day file" << _file.errorString();
        return {};
    }

    auto dateString = date.toString("0004-MM-dd").toUtf8();

    while (!_file.atEnd()) {
        auto line = _file.readLine();

        if (!line.startsWith(dateString))
            continue;

        QList<ActionDay *> days;

        while (true) {
            if (!line.startsWith(dateString))
                break;

            auto sections = line.split(',');
            Q_ASSERT(sections.size() == 4);

            auto day = new ActionDay{parent};
            day->setDate(QDate::fromString(sections[0], "yyyy-MM-dd"));
            day->setName(sections[1]);
            day->setDesc(sections[2]);
            day->setLink(sections[3]);
            days.append(day);
            line = _file.readLine();
        }

        qCInfo(self) << "found" << days.size() << "action days for" << date;
        _file.close();
        return days;
    }

    _file.close();
    return {};
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
