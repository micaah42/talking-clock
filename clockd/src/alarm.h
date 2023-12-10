/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * alarm.h                                                               *
 *                                                                       *
 * Alarm implements the logic of repetition of alarms on weekdays and    *
 * can return the next DateTime this alarm should be triggered.          *
 * - michael, 2022-03                                                    *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef ALARM_H
#define ALARM_H

#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>
#include <QTime>

class Alarm
{
    Q_GADGET

    Q_PROPERTY(bool activated READ activated WRITE setActivated)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QTime time READ time WRITE setTime)
    Q_PROPERTY(QVariantList repeatRule READ repeatRule WRITE setRepeatRule)
    Q_PROPERTY(QString sound READ sound WRITE setSound)
    Q_PROPERTY(QString program READ program WRITE setProgram)
    Q_PROPERTY(QStringList arguments READ arguments WRITE setArguments)

public:
    explicit Alarm();
    ~Alarm(){};

    Q_INVOKABLE QDateTime nextTrigger(const QDateTime &after) const;
    bool repeats() const;

    const QString &name() const;
    void setName(const QString &newName);

    QTime time() const;
    void setTime(const QTime &newTime);

    QVariantList repeatRule() const;
    void setRepeatRule(const QVariantList &newRepeat);

    bool activated() const;
    void setActivated(bool newActivated);

    const QString &sound() const;
    void setSound(const QString &newSound);

    const QString &program() const;
    void setProgram(const QString &newProgram);

    const QStringList &arguments() const;
    void setArguments(const QStringList &newArguments);

private:
    // whether or not the alarm is activated
    bool _activated;
    // name or description
    QString _name;
    // time to trigger
    QTime _time;
    // on which days to trigger
    std::array<bool, 7> _repeatRule;

    QString _sound;
    QString _program;
    QStringList _arguments;
};
Q_DECLARE_METATYPE(Alarm)

QDebug operator<<(QDebug debug, const Alarm &c);

#endif // ALARM_H
