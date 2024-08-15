#ifndef ALARM_H
#define ALARM_H

#include <QDateTime>
#include <QObject>
#include <QTime>
#include <QTimer>

class Alarm : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool activated READ activated WRITE setActivated NOTIFY activatedChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QTime time READ time WRITE setTime NOTIFY timeChanged FINAL)
    Q_PROPERTY(QVariantList repeatRule READ repeatRule WRITE setRepeatRule NOTIFY repeatRuleChanged FINAL)
    Q_PROPERTY(bool repeats READ repeats NOTIFY repeatsChanged FINAL)
    Q_PROPERTY(QString sound READ sound WRITE setSound NOTIFY soundChanged FINAL)
    Q_PROPERTY(QDateTime nextTimeout READ nextTimeout NOTIFY nextTimeoutChanged FINAL)

public:
    Q_INVOKABLE explicit Alarm(QObject *parent = nullptr);

    bool activated() const;
    void setActivated(bool newActivated);

    QString name() const;
    void setName(const QString &newName);

    QTime time() const;
    void setTime(const QTime &newTime);

    QVariantList repeatRule() const;
    void setRepeatRule(const QVariantList &newRepeatRule);

    QString sound() const;
    void setSound(const QString &newSound);

    QDateTime nextTimeout() const;
    bool repeats() const;

public slots:
    void findNextTimeout(QDateTime after = QDateTime::currentDateTime());
    void snooze(int minutes);

signals:
    void activatedChanged();
    void nameChanged();
    void timeChanged();
    void repeatRuleChanged();
    void soundChanged();
    void nextTimeoutChanged();
    void repeatsChanged();

protected:
    void setNextTimeout(const QDateTime &newNextTimeout);
    void setRepeats(bool newRepeats);

private:
    bool _activated;
    QString _name;
    QTime _time;
    QList<bool> _repeatRule;
    QString _sound;
    QDateTime _nextTimeout;
    bool _repeats;

    QTimer _timer;
};

QDebug operator<<(QDebug debug, const Alarm &c);

#endif // ALARM_H
