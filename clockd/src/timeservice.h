#ifndef TIMESERVICE_H
#define TIMESERVICE_H

#include <QDate>
#include <QDateTime>
#include <QObject>
#include <QQmlEngine>
#include <QTimer>

class TimeService : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    // now, precise to the second
    Q_PROPERTY(QDateTime now READ now NOTIFY nowChanged FINAL)

    // today, changes date exactly
    Q_PROPERTY(QDate today READ today NOTIFY todayChanged FINAL)

    // now, but only updated when the minute changes
    Q_PROPERTY(QDateTime nowMinutes READ nowMinutes NOTIFY nowMinutesChanged FINAL)

    // now, but only updated every 15 minutes
    Q_PROPERTY(QDateTime now15Minutes READ now15Minutes NOTIFY now15MinutesChanged FINAL)

    // now, but only updated every hours
    Q_PROPERTY(QDateTime nowHours READ nowHours NOTIFY nowHoursChanged FINAL)

public:
    explicit TimeService(QObject *parent = nullptr);

    QDateTime now() const;
    QDate today() const;
    QDateTime nowMinutes() const;
    QDateTime now15Minutes() const;
    QDateTime nowHours() const;

signals:
    void nowChanged();
    void todayChanged();
    void nowMinutesChanged();
    void now15MinutesChanged();
    void nowHoursChanged();

protected:
    void setNow(const QDateTime &newNow);
    void setToday(const QDate &newToday);
    void setNowMinutes(const QDateTime &newNowMinutes);
    void setNow15Minutes(const QDateTime &newNow15Minutes);
    void setNowHours(const QDateTime &newNowHours);

    void onTimeout();

private:
    QDateTime _now;
    QDate _today;
    QDateTime _nowMinutes;
    QDateTime _now15Minutes;
    QDateTime _nowHours;

    QTimer _timer;
    int _overshoot;
};

#endif // TIMESERVICE_H
