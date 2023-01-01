#include "alarm.h"

#include <QDebug>
#include <QJsonArray>
#include <QLoggingCategory>

#include "jsonspecification.h"

namespace {
Q_LOGGING_CATEGORY(self, "alarm");
}

Alarm::Alarm() : _activated{true}, _time{QTime::currentTime()}
{
    _repeatRule.fill(false);
}

QTime Alarm::time() const
{
    return _time;
}

void Alarm::setTime(const QTime &newTime)
{
    _time = newTime;
}

QVariantList Alarm::repeatRule() const
{
    QVariantList ret;
    for (auto val : _repeatRule) {
        ret.append(val);
    }
    return ret;
}

void Alarm::setRepeatRule(const QVariantList &newRepeat)
{
    Q_ASSERT(newRepeat.size() == 7);
    for (int i = 0; i < 7; ++i) {
        // Q_ASSERT(newRepeat[i].isBool());
        _repeatRule[i] = newRepeat[i].toBool();
    }
}

int Alarm::nextTrigger(const QDateTime &after) const
{
    if (!_activated) {
        return -1;
    }
    QDateTime nextTime{after.date(), _time};

    // if time has already passed today, next trigger can only be tomorrow or onwards
    if (_time <= after.time())
        nextTime = nextTime.addDays(1);

    // now we skip days until the rules allow us to trigger on that date
    // note: if no day is selected we remain at tomorrow / today

    if (repeats())
        while (_repeatRule[nextTime.date().dayOfWeek() - 1] == false) {
            nextTime = nextTime.addDays(1);
        }

    // return how long from now this is away
    return QDateTime::currentDateTime().msecsTo(nextTime);
}

bool Alarm::repeats() const
{
    for (auto val : _repeatRule) {
        if (val) {
            return true;
        }
    }
    return false;
}

bool Alarm::activated() const
{
    return _activated;
}

void Alarm::setActivated(bool newActivated)
{
    _activated = newActivated;
}

const QString &Alarm::name() const
{
    return _name;
}

void Alarm::setName(const QString &newName)
{
    _name = newName;
}

const QString &Alarm::sound() const
{
    return _sound;
}

void Alarm::setSound(const QString &newSound)
{
    _sound = newSound;
}

const QString &Alarm::program() const
{
    return _program;
}

void Alarm::setProgram(const QString &newProgram)
{
    _program = newProgram;
}

const QStringList &Alarm::arguments() const
{
    return _arguments;
}

void Alarm::setArguments(const QStringList &newArguments)
{
    _arguments = newArguments;
}
