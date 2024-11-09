#ifndef ACTIONDAY_H
#define ACTIONDAY_H

#include <QDate>
#include <QFile>
#include <QObject>
#include <QTimer>

class ActionDay;

class ActionDayService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<ActionDay *> days READ days NOTIFY daysChanged FINAL)
public:
    explicit ActionDayService(QObject *parent = nullptr);

    const QList<ActionDay *> &days() const;

public slots:
    QList<ActionDay *> actionDays(const QDate &date = QDate::currentDate(), QObject *parent = nullptr);
    void onTimeout();

signals:
    void daysChanged();

private:
    QFile _file;
    QTimer _timer;
    QList<ActionDay *> _days;
};

class ActionDay : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDate date READ date WRITE setDate NOTIFY dateChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString desc READ desc WRITE setDesc NOTIFY descChanged FINAL)
    Q_PROPERTY(QString link READ link WRITE setLink NOTIFY linkChanged FINAL)

public:
    explicit ActionDay(QObject *parent = nullptr);

    QDate date() const;
    void setDate(const QDate &newDate);

    QString name() const;
    void setName(const QString &newName);

    QString desc() const;
    void setDesc(const QString &newDesc);

    QString link() const;
    void setLink(const QString &newLink);

signals:
    void dateChanged();
    void nameChanged();
    void descChanged();
    void linkChanged();

private:
    QDate m_date;
    QString m_name;
    QString m_desc;
    QString m_link;
};
#endif // ACTIONDAY_H
