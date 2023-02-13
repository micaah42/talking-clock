#ifndef ALARMSERVICE_H
#define ALARMSERVICE_H

#include <QFile>
#include <QJsonArray>
#include <QMap>
#include <QObject>
#include <QTimer>

#include "alarm.h"
#include "templatemodel.h"

#define MAX_ALARMS 1024
#define ALARMS_FILE "./alarms.json"

DECLARE_MODEL(Alarm)

class AlarmService : public QObject
{
    Q_OBJECT
public:
    AlarmService(const int tickRate = 500, QObject *parent = nullptr);

    QList<int> nextIds() const;
    const QString &clockString() const;

    QJsonArray nextIdsArray() const;

    //const QMap<int, Alarm> &alarms() const;

    AlarmModel *model();

signals:
    void alarmTriggered(const int id);
    void clockTicked(const QString &now);
    void nextIdsChanged();

private slots:
    void onTriggererTriggered();
    void onClockTriggered();

    void saveAlarms();
    void loadAlarms();

private:
    QTimer _triggerer;

    QTimer _clock;
    QTimer _saveTimer;
    QString _clockString;
    QString _clockFormat;

    QList<int> _nextIds;
    void updateTriggerer(const QDateTime &after = QDateTime::currentDateTime());

    AlarmModel _alarms;
    QFile _alarmsFile;

    // properties
    Q_PROPERTY(QString clockString READ clockString NOTIFY clockTicked)
    Q_PROPERTY(QJsonArray nextIds READ nextIdsArray NOTIFY nextIdsChanged)
    Q_PROPERTY(AlarmModel *model READ model CONSTANT)
};

#endif // ALARMSERVICE_H
