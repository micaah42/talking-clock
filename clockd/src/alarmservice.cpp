#include "alarmservice.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QLoggingCategory>

#include "pathservice.h"
#include "serializer.h"

namespace {
Q_LOGGING_CATEGORY(self, "alarms")
}

AlarmService::AlarmService(const int tickRate, QObject *parent) : QObject(parent)
{
    // set up the alarm triggerer
    connect(&_triggerer, &QTimer::timeout, this, &AlarmService::onTriggererTriggered);
    _triggerer.setTimerType(Qt::CoarseTimer);
    _triggerer.setSingleShot(true);

    // and the clock refresher
    connect(&_clock, &QTimer::timeout, this, &AlarmService::onClockTriggered);
    _clock.setInterval(tickRate);
    _clock.setSingleShot(false);
    _clock.start();

    // set clock format
    _clockFormat = Qt::ISODate;

    // load arlams data
    _alarmsFile.setFileName(PathService::homeFile("alarms.json"));
    loadAlarms();

    // save changes to file
    connect(&_alarms,
            &AlarmModel::dataChanged,
            this,
            [this](const QModelIndex &index, const QVariant &value, const QVector<int> role) {
                updateTriggerer();
                saveAlarms();
            });
}

QList<int> AlarmService::nextIds() const
{
    return _nextIds;
}

void AlarmService::updateTriggerer()
{
    auto now = QDateTime::currentDateTime();
    int minTimeout = -1;

    QList<int> nextIds;
    for (int i = 0; i < _alarms.size(); i++) {
        const auto &alarm = _alarms.at(i);
        auto alarmNextTrigger = alarm.nextTrigger(now);
        if (alarmNextTrigger == -1) {
            continue;
        }
        else if (alarmNextTrigger < minTimeout || minTimeout == -1) {
            nextIds.clear();
            nextIds.append(i);
            minTimeout = alarmNextTrigger;
        }
        else if (alarmNextTrigger == minTimeout) {
            nextIds.append(i);
        }
    }

    if (nextIds != _nextIds) {
        qCInfo(self) << "nextIds:" << _nextIds << "->" << nextIds;
        _nextIds = nextIds;
        emit nextIdsChanged();
    }

    if (minTimeout == -1) {
        qCInfo(self) << "no next alarm. deactivate triggerer";
        _triggerer.stop();
    }
    else {
        qCInfo(self) << "trigger in:" << minTimeout / (1000 * 60 * 60) << "h";
        _triggerer.setInterval(minTimeout);
        _triggerer.start();
    }
}

void AlarmService::onTriggererTriggered()
{
    auto now = QDateTime::currentDateTime();
    for (auto id : qAsConst(_nextIds)) {
        if (!_alarms.at(id).repeats()) {
            auto deactivated = _alarms.at(id);
            deactivated.setActivated(false);
            _alarms.set(id, deactivated);
        }
        qCInfo(self) << "triggered alarm:" << id << "with drift:" << _alarms.at(id).time();
        emit alarmTriggered(id);
    }

    updateTriggerer();
}

void AlarmService::onClockTriggered()
{
    QString _before = _clockString;
    _clockString = QDateTime::currentDateTime().toString(Qt::ISODate);
    if (_before != _clockString) {
        emit clockTicked(_clockString);
    }
}

void AlarmService::saveAlarms()
{
    if (!_alarmsFile.open(QIODevice::WriteOnly)) {
        qCWarning(self) << "failed to open alarms file for saving!";
        return;
    }

    QJsonArray alarms;
    for (int i = 0; i < _alarms.size(); ++i) {
        Alarm alarm = _alarms.at(i);
        alarms.append(VariantSerializer::I()->serialize(alarm));
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
        qCWarning(self) << error.errorString();
        _alarmsFile.close();
        return;
    }

    // insert alarms
    if (alarms.isArray()) {
        for (auto const &alarmJson : alarms.array()) {
            auto alarm = VariantSerializer::I()->deserialize<Alarm>(alarmJson);
            if (true) {
                qCInfo(self) << "loaded:" << alarmJson;
                _alarms.push(alarm);
            }
        }
    }
    _alarmsFile.close();
}

const QString &AlarmService::clockString() const
{
    return _clockString;
}

QJsonArray AlarmService::nextIdsArray() const
{
    QJsonArray ids;
    for (auto const id : _nextIds) {
        ids.append(id);
    }
    return ids;
}

AlarmModel *AlarmService::model()
{
    return &_alarms;
}