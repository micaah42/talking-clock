#ifndef TIMEZONEMODEL_H
#define TIMEZONEMODEL_H

#include <QObject>
#include <QTimeZone>
#include <qlistmodel.h>

class TimeZone : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QByteArray id READ id CONSTANT FINAL)
    Q_PROPERTY(QLocale::Territory territory READ territory CONSTANT FINAL)
    Q_PROPERTY(QString comment READ comment CONSTANT FINAL)
    Q_PROPERTY(bool hasDaylightTime READ hasDaylightTime CONSTANT FINAL)

public:
    enum TimeType {
        StandardTime = 0,
        DaylightTime = 1,
        GenericTime = 2,
    };
    Q_ENUM(TimeType);

    enum NameType {
        DefaultName = 0,
        LongName = 1,
        ShortName = 2,
        OffsetName = 3,
    };
    Q_ENUM(NameType);

    explicit TimeZone(const QByteArray &id, QObject *parent = nullptr);
    QByteArray id() const;
    QLocale::Territory territory() const;
    QString comment() const;
    QString abbreviation(const QDateTime &atDateTime) const;
    bool hasDaylightTime() const;
    bool hasTransitions() const;

public slots:
    QString displayName(const QDateTime &atDateTime, int nameType = QTimeZone::DefaultName, const QLocale &locale = QLocale()) const;
    int offsetFromUtc(const QDateTime &atDateTime) const;
    int standardTimeOffset(const QDateTime &atDateTime) const;
    int daylightTimeOffset(const QDateTime &atDateTime) const;
    bool isDaylightTime(const QDateTime &atDateTime) const;

private:
    QTimeZone _timeZone;
};

class TimeZoneModel : public QListModel<TimeZone *>
{
    Q_OBJECT
public:
    explicit TimeZoneModel(QObject *parent = nullptr);

signals:
private:
};

#endif // TIMEZONEMODEL_H
