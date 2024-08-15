#ifndef ALARMSERVICE_H
#define ALARMSERVICE_H

#include <QFile>
#include <QJsonArray>
#include <QMap>
#include <QObject>
#include <QTimer>

#include <qlistmodel.h>

#include "alarm.h"

#define MAX_ALARMS 1024
#define ALARMS_FILE "./alarms.json"

class AlarmService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Alarm *nextAlarm READ nextAlarm NOTIFY nextAlarmChanged FINAL)
    Q_PROPERTY(QListModelBase *model READ model CONSTANT)
    Q_PROPERTY(QDateTime now READ now NOTIFY clockTicked)

public:
    AlarmService(const int tickRate = 500, QObject *parent = nullptr);

    const QDateTime &now() const;
    QListModel<Alarm *> *model();
    Alarm *nextAlarm() const;

public slots:
    void removeAlarm(Alarm *alarm);
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

    QListModel<Alarm *> _alarmModel;
    QFile _alarmsFile;
    Alarm *_nextAlarm;
};

#endif // ALARMSERVICE_H
