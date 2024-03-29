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

    // set up the save timer
    connect(&_persistTimer, &QTimer::timeout, this, &AlarmService::saveAlarms);
    _persistTimer.setSingleShot(true);
    _persistTimer.setInterval(500);

    // handle changes in the alarms
    connect(&_alarms, &AlarmModel::dataChanged, this, [this](const QModelIndex &index, const QVariant &value, const QVector<int> role) {
        _persistTimer.start();
    });

    connect(&_alarms, &AlarmModel::rowsRemoved, this, [this](const QModelIndex &parent, const int &first, const int last) { _persistTimer.start(); });

    // load arlams data
    _alarmsFile.setFileName(PathService::create("alarms.json"));
    loadAlarms();
}

QList<int> AlarmService::nextIds() const
{
    return _nextIds;
}

void AlarmService::updateTriggerer(const QDateTime &after)
{
    QDateTime next;

    // find next ids
    QList<int> nextIds;
    for (int i = 0; i < _alarms.size(); i++) {
        auto nextTimout = _alarms.at(i).nextTrigger(after);

        if (!nextTimout.isValid()) {
            continue;
        }

        else if (nextTimout < next || !next.isValid()) {
            nextIds.clear();
            nextIds.append(i);
            next = nextTimout;
        }

        else if (nextTimout == next) {
            nextIds.append(i);
        }
    }

    // set timer for next alarm according to next ids

    if (nextIds != _nextIds) {
        qCInfo(self) << "nextIds:" << _nextIds << "->" << nextIds;
        _nextIds = nextIds;
        emit nextIdsChanged();
    }

    if (!next.isValid()) {
        qCInfo(self) << "no next alarm. deactivate triggerer";
        _triggerer.stop();
    }

    else {
        qCInfo(self) << "trigger at:" << next;
        auto now = QDateTime::currentDateTime();
        _triggerer.setInterval(now.msecsTo(next));
        _triggerer.start();
    }
}

void AlarmService::onTriggererTriggered()
{
    QDateTime after;
    auto now = QDateTime::currentDateTime();
    for (auto id : qAsConst(_nextIds)) {
        //
        if (!_alarms.at(id).repeats()) {
            auto deactivated = _alarms.at(id);
            deactivated.setActivated(false);
            _alarms.set(id, deactivated);
        }

        after = QDateTime{now.date(), _alarms.at(id).time()};
        auto drift = now.time().msecsTo(_alarms.at(id).time());
        qCInfo(self) << "triggered alarm:" << id << "with drift:" << drift;
        emit alarmTriggered(id);
    }

    after = after.isValid() ? after : now;
    updateTriggerer(after);
}

void AlarmService::onClockTriggered()
{
    _now = QDateTime::currentDateTime();
    emit clockTicked();
}

void AlarmService::saveAlarms()
{
    updateTriggerer();

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
    for (auto const &alarmJson : qAsConst(alarmsArray)) {
        auto alarm = VariantSerializer::I()->deserialize<Alarm>(alarmJson);
        if (true) {
            qCInfo(self) << "loaded:" << alarmJson;
            _alarms.push(alarm);
        }
    }

    _alarmsFile.close();
}

const QDateTime &AlarmService::now() const
{
    return _now;
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
