#ifndef ACTIONDAY_H
#define ACTIONDAY_H

#include <QDate>
#include <QFile>
#include <QObject>
#include <QQmlEngine>
#include <QTimer>

#include "model.h"

class ActionDay : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    // Q_PROPERTY(QDate date READ date WRITE setDate NOTIFY dateChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString desc READ desc WRITE setDesc NOTIFY descChanged FINAL)
    Q_PROPERTY(QString link READ link WRITE setLink NOTIFY linkChanged FINAL)
    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged FINAL)

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

    QString icon() const;
    void setIcon(const QString &newIcon);

signals:
    void dateChanged();
    void nameChanged();
    void descChanged();
    void linkChanged();
    void iconChanged();

private:
    QDate m_date;
    QString m_name;
    QString m_desc;
    QString m_link;
    QString m_icon;
};

class ActionDayService : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

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
    Model<ActionDay> _model;
    QList<ActionDay *> _days;
};
#endif // ACTIONDAY_H
