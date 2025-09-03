#ifndef ALARMSERVICE_H
#define ALARMSERVICE_H

#include <QFile>
#include <QJsonArray>
#include <QMap>
#include <QObject>
#include <QQmlEngine>
#include <QTimer>

#include <listmodel.h>
#include <sortfilterproxylistmodel.h>

#include "alarm.h"

#define MAX_ALARMS 1024
#define ALARMS_FILE "./alarms.json"

class SortFilterAlarmModel : public SortFilterProxyListModel<Alarm *>
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit SortFilterAlarmModel();

public:
    virtual bool filterAcceptsRow(int index, Alarm *t) const override { return true; };
    virtual bool lessThan(Alarm *a, Alarm *b) const override
    {
        if (!a->nextTimeout().isValid())
            return false;

        return a->nextTimeout() < b->nextTimeout();
    };
};

class AlarmService : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(Alarm *nextAlarm READ nextAlarm NOTIFY nextAlarmChanged FINAL)
    Q_PROPERTY(ListModelBase *model READ model CONSTANT)
    Q_PROPERTY(QDateTime now READ now NOTIFY clockTicked)

public:
    AlarmService(const int tickRate = 500, QObject *parent = nullptr);

    const QDateTime &now() const;
    ListModel<Alarm *> *model();
    Alarm *nextAlarm() const;

public slots:
    void removeAlarm(Alarm *alarm);
    void addAlarm(Alarm *alarm);
    Alarm *newAlarm();

signals:
    void alarmTriggered(Alarm *);
    void nextAlarmChanged();
    void clockTicked();

protected:
    void setNextAlarm(Alarm *newNextAlarm);

private slots:
    void onClockTriggered();
    void onTimeoutChanged();

    void registerAlarm(Alarm *alarm);
    void saveAlarms();
    void loadAlarms();

private:
    QTimer _clock;
    QDateTime _now;
    QTimer _saveTimer;

    QMap<Alarm *, QDateTime> _timeoutMap;
    QMultiMap<QDateTime, Alarm *> _alarmQueue;

    ListModel<Alarm *> _alarmModel;
    QFile _alarmsFile;
    Alarm *_nextAlarm;
};

#endif // ALARMSERVICE_H
