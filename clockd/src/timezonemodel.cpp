#include "timezonemodel.h"

TimeZone::TimeZone(const QByteArray &id, QObject *parent)
    : QObject{parent}
    , _timeZone{id}
{}

QByteArray TimeZone::id() const
{
    return _timeZone.id();
}

QLocale::Territory TimeZone::territory() const
{
    return _timeZone.territory();
}

QString TimeZone::comment() const
{
    return _timeZone.comment();
}

QString TimeZone::abbreviation(const QDateTime &atDateTime) const
{
    return _timeZone.abbreviation(atDateTime);
}

bool TimeZone::hasDaylightTime() const
{
    return _timeZone.hasDaylightTime();
}

bool TimeZone::hasTransitions() const
{
    return _timeZone.hasTransitions();
}

QString TimeZone::displayName(const QDateTime &atDateTime, int nameType, const QLocale &locale) const
{
    return _timeZone.displayName(atDateTime, static_cast<QTimeZone::NameType>(nameType), locale);
}

int TimeZone::offsetFromUtc(const QDateTime &atDateTime) const
{
    return _timeZone.offsetFromUtc(atDateTime);
}

int TimeZone::standardTimeOffset(const QDateTime &atDateTime) const
{
    return _timeZone.standardTimeOffset(atDateTime);
}

int TimeZone::daylightTimeOffset(const QDateTime &atDateTime) const
{
    return _timeZone.daylightTimeOffset(atDateTime);
}

bool TimeZone::isDaylightTime(const QDateTime &atDateTime) const
{
    return _timeZone.isDaylightTime(atDateTime);
}

TimeZoneModel::TimeZoneModel(QObject *parent)
    : QListModel<TimeZone *>{parent}
{
    QList<TimeZone *> tzs;
    const auto timeZoneIds = {
        "Indian/Reunion",
        "Indian/Mauritius",
        "Indian/Antananarivo",
        "Indian/Comoro",
        "Indian/Cocos",
        "Indian/Christmas",
        "Indian/Maldives",
        "Indian/Kerguelen",
        "Indian/Chagos",
        "Indian/Mahe",
        "Indian/Mayotte",

    }; //QTimeZone::availableTimeZoneIds();

    for (auto const id : timeZoneIds)
        tzs.append(new TimeZone(id, this));

    this->append(tzs);
}
