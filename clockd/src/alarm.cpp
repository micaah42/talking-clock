#include "alarm.h"

#include <QLoggingCategory>

#include "qlistmodel.h"

namespace {
Q_LOGGING_CATEGORY(self, "alarm");
}

Alarm::Alarm(QObject *parent)
    : QObject{parent}
    , _activated{true}
    , _time{12, 0}
    , _repeats{false}

{
    _repeatRule.fill(false, 7);

    _timer.setSingleShot(true);
    _timer.setInterval(500);

    connect(&_timer, &QTimer::timeout, this, [this]() { this->findNextTimeout(QDateTime::currentDateTime()); });
    this->findNextTimeout(QDateTime::currentDateTime());

    connect(this, &Alarm::activatedChanged, &_timer, qOverload<>(&QTimer::start));
    connect(this, &Alarm::timeChanged, &_timer, qOverload<>(&QTimer::start));
    connect(this, &Alarm::repeatRuleChanged, &_timer, qOverload<>(&QTimer::start));
}

bool Alarm::activated() const
{
    return _activated;
}

void Alarm::setActivated(bool newActivated)
{
    if (_activated == newActivated)
        return;

    _activated = newActivated;
    emit activatedChanged();
}

QString Alarm::name() const
{
    return _name;
}

void Alarm::setName(const QString &newName)
{
    if (_name == newName)
        return;

    _name = newName;
    emit nameChanged();
}

QTime Alarm::time() const
{
    return _time;
}

void Alarm::setTime(const QTime &newTime)
{
    if (_time == newTime)
        return;

    _time = newTime;
    emit timeChanged();
}

QVariantList Alarm::repeatRule() const
{
    return qToVariantList(_repeatRule);
}

void Alarm::setRepeatRule(const QVariantList &newRepeatRule)
{
    auto repeatRule = qFromVariantList<bool>(newRepeatRule);
    if (_repeatRule == repeatRule)
        return;

    _repeatRule = repeatRule;
    emit repeatRuleChanged();

    bool repeats = std::any_of(_repeatRule.begin(), _repeatRule.end(), [](bool a) { return a; });
    this->setRepeats(repeats);
}

QString Alarm::sound() const
{
    return _sound;
}

void Alarm::setSound(const QString &newSound)
{
    if (_sound == newSound)
        return;

    _sound = newSound;
    emit soundChanged();
}

QDateTime Alarm::nextTimeout() const
{
    return _nextTimeout;
}

void Alarm::setNextTimeout(const QDateTime &newNextTimeout)
{
    if (_nextTimeout == newNextTimeout)
        return;

    _nextTimeout = newNextTimeout;
    emit nextTimeoutChanged();
}

bool Alarm::repeats() const
{
    return _repeats;
}

void Alarm::findNextTimeout(QDateTime after)
{
    qCInfo(self) << "find next timeout for:" << *this << "after" << after;

    if (!_activated) {
        qCInfo(self) << "no timeout";
        this->setNextTimeout(QDateTime());
        return;
    }

    QDateTime nextTimeout{after.date(), _time};

    // if time has already passed today, next trigger can only be tomorrow or onwards

    if (_time <= after.time())
        nextTimeout = nextTimeout.addDays(1);

    // now we skip days until the rules allow us to trigger on that date
    // note: if no day is selected we remain at tomorrow / today

    if (repeats())
        while (_repeatRule[nextTimeout.date().dayOfWeek() - 1] == false) {
            nextTimeout = nextTimeout.addDays(1);
        }

    qCInfo(self) << "next timeout for:" << *this << ":" << nextTimeout;
    this->setNextTimeout(nextTimeout);
}

void Alarm::snooze(int minutes)
{
    auto newTimeout = QDateTime::currentDateTime().addSecs(60 * minutes);
    this->setNextTimeout(newTimeout);
}

void Alarm::setRepeats(bool newRepeats)
{
    if (_repeats == newRepeats)
        return;

    _repeats = newRepeats;
    emit repeatsChanged();
}

QDebug operator<<(QDebug debug, const Alarm &c)
{
    QDebugStateSaver saver(debug);
    QString templ{"Alarm(activated=%1,name='%2',time='%3',sound='%5')"};
    debug << templ.arg(c.activated() ? "true" : "false", c.name(), c.time().toString(), c.sound());
    return debug;
}
