#include "alarmservice.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QLoggingCategory>

#include "json.h"

#include "pathservice.h"

namespace {
Q_LOGGING_CATEGORY(self, "alarms")
}

AlarmService::AlarmService(const int tickRate, QObject *parent)
    : QObject(parent)
    , _nextAlarm{nullptr}
{
    connect(&_clock, &QTimer::timeout, this, &AlarmService::onClockTriggered);
    _clock.setInterval(tickRate);
    _clock.start();

    connect(&_saveTimer, &QTimer::timeout, this, &AlarmService::saveAlarms);
    _saveTimer.setSingleShot(true);
    _saveTimer.setInterval(500);

    _alarmsFile.setFileName(PathService::create("alarms.json"));
    this->loadAlarms();
}

void AlarmService::onClockTriggered()
{
    _now = QDateTime::currentDateTime();
    emit clockTicked();

    if (_alarmQueue.empty())
        return;

    QDateTime nextTimeout = _alarmQueue.firstKey();

    while (!_alarmQueue.empty() && nextTimeout < _now) {
        const auto alarm = _alarmQueue.first();

        _alarmQueue.remove(alarm->nextTimeout(), alarm);
        _timeoutMap.remove(alarm);

        if (!alarm->repeats()) {
            alarm->setActivated(false);
        }

        auto drift = _now.msecsTo(alarm->nextTimeout());
        qCInfo(self) << "triggered alarm:" << *alarm << "with drift:" << drift;

        emit alarmTriggered(alarm);

        alarm->findNextTimeout(nextTimeout);

        if (!_alarmQueue.empty())
            nextTimeout = _alarmQueue.firstKey();
    }
}

void AlarmService::onTimeoutChanged()
{
    auto alarm = qobject_cast<Alarm *>(sender());

    qCInfo(self) << "new next timeout from:" << alarm << alarm->nextTimeout();

    auto oldTime = *_timeoutMap.find(alarm);

    _alarmQueue.remove(oldTime, alarm);
    _timeoutMap.remove(alarm);

    if (alarm->nextTimeout().isValid()) {
        _alarmQueue.insert(alarm->nextTimeout(), alarm);
        _timeoutMap.insert(alarm, alarm->nextTimeout());
    }

    if (!_alarmQueue.empty())
        this->setNextAlarm(_alarmQueue.first());
    else
        this->setNextAlarm(nullptr);
}

void AlarmService::registerAlarm(Alarm *alarm)
{
    /* persist alarms if property changed */

    connect(alarm, &Alarm::activatedChanged, &_saveTimer, qOverload<>(&QTimer::start));
    connect(alarm, &Alarm::nameChanged, &_saveTimer, qOverload<>(&QTimer::start));
    connect(alarm, &Alarm::timeChanged, &_saveTimer, qOverload<>(&QTimer::start));
    connect(alarm, &Alarm::repeatRuleChanged, &_saveTimer, qOverload<>(&QTimer::start));
    connect(alarm, &Alarm::soundChanged, &_saveTimer, qOverload<>(&QTimer::start));
    _saveTimer.start();

    /* set up scheduling */

    if (alarm->nextTimeout().isValid()) {
        _alarmQueue.insert(alarm->nextTimeout(), alarm);
        _timeoutMap.insert(alarm, alarm->nextTimeout());
    }

    connect(alarm, &Alarm::nextTimeoutChanged, this, &AlarmService::onTimeoutChanged);

    connect(alarm, &Alarm::destroyed, this, [this, alarm]() {
        _alarmQueue.remove(alarm->nextTimeout());
        _timeoutMap.remove(alarm);

        if (!_alarmQueue.empty())
            this->setNextAlarm(_alarmQueue.first());
        else
            this->setNextAlarm(nullptr);
    });
}

void AlarmService::saveAlarms()
{
    if (!_alarmsFile.open(QIODevice::WriteOnly)) {
        qCWarning(self) << "failed to open alarms file for saving!";
        return;
    }

    QJsonArray alarms;

    for (int i = 0; i < _alarmModel.size(); i++) {
        Alarm *alarm = _alarmModel.at(i);
        alarms.append(JSON::serialize(alarm));
        qCDebug(self) << "serialized alarm:" << *alarm << alarms.last();
    }

    auto data = QJsonDocument(alarms).toJson(QJsonDocument::Indented);
    _alarmsFile.write(data);
    _alarmsFile.close();

    qCInfo(self) << "saved alarms!";
}

void AlarmService::loadAlarms()
{
    qCInfo(self) << "loading alarms ...";

    // check wether we can open the file
    if (!_alarmsFile.open(QIODevice::ReadOnly)) {
        qCWarning(self) << "could not open alarms file:" << _alarmsFile.errorString();
        return;
    }

    // read file and parse
    QJsonParseError error;
    auto data = _alarmsFile.readAll();
    auto alarms = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        qCWarning(self) << "alarm parsing error:" << error.errorString();
        _alarmsFile.close();
        return;
    }

    if (!alarms.isArray()) {
        qCWarning(self) << "alarms json is not an array!";
        _alarmsFile.close();
        return;
    }

    auto alarmsArray = alarms.array();

    for (auto const &alarmJson : std::as_const(alarmsArray)) {
        auto alarm = JSON::deserialize<Alarm *>(alarmJson);

        if (!alarm) {
            qCCritical(self) << "failed to load alarm from:" << alarmJson;
            continue;
        }

        qCInfo(self) << "loaded:" << *alarm << alarmJson;
        this->registerAlarm(alarm);
        _alarmModel.append(alarm);
        alarm->setParent(this);
    }

    if (!_alarmQueue.empty())
        this->setNextAlarm(_alarmQueue.first());
    else
        this->setNextAlarm(nullptr);

    _alarmsFile.close();
}

const QDateTime &AlarmService::now() const
{
    return _now;
}

QListModel<Alarm *> *AlarmService::model()
{
    return &_alarmModel;
}

Alarm *AlarmService::nextAlarm() const
{
    return _nextAlarm;
}

void AlarmService::removeAlarm(Alarm *alarm)
{
    _alarmModel.removeAll(alarm);
    alarm->deleteLater();
}

void AlarmService::addAlarm(Alarm *alarm)
{
    this->registerAlarm(alarm);
    _alarmModel.append(alarm);

    if (!_alarmQueue.empty())
        this->setNextAlarm(_alarmQueue.first());
    else
        this->setNextAlarm(nullptr);
}

Alarm *AlarmService::newAlarm()
{
    auto alarm = new Alarm{this};
    return alarm;
}

void AlarmService::setNextAlarm(Alarm *newNextAlarm)
{
    if (_nextAlarm == newNextAlarm)
        return;

    qCInfo(self) << "new next alarm:" << newNextAlarm;
    _nextAlarm = newNextAlarm;
    emit nextAlarmChanged();
}
