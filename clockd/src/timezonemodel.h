#ifndef TIMEZONEMODEL_H
#define TIMEZONEMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>
#include <QTimeZone>
#include <qlistmodel.h>

class QProcess;

class TimeZone : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id CONSTANT FINAL)
    Q_PROPERTY(QString territory READ territory CONSTANT FINAL)
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
    QString id() const;
    QString territory() const;
    QString comment() const;
    QString abbreviation(const QDateTime &atDateTime) const;
    bool hasDaylightTime() const;
    bool hasTransitions() const;
    bool isValid() const;
    const QString &searchString() const;

public slots:
    QString displayName(const QDateTime &atDateTime = QDateTime(), int nameType = QTimeZone::DefaultName, const QLocale &locale = QLocale()) const;
    int offsetFromUtc(const QDateTime &atDateTime = QDateTime()) const;
    int standardTimeOffset(const QDateTime &atDateTime) const;
    int daylightTimeOffset(const QDateTime &atDateTime) const;
    bool isDaylightTime(const QDateTime &atDateTime) const;
    QDateTime convertDateTime(const QDateTime &dateTime);

private:
    QTimeZone _timeZone;
    QString _searchString;
};

class TimeZoneModel : public QListModel<TimeZone *>
{
    Q_OBJECT
    Q_PROPERTY(bool refreshing READ refreshing WRITE setRefreshing NOTIFY refreshingChanged FINAL)
    Q_PROPERTY(TimeZone *systemTimeZone READ systemTimeZone WRITE setSystemTimeZone NOTIFY systemTimeZoneChanged FINAL)
public:
    explicit TimeZoneModel(QObject *parent = nullptr);

    bool refreshing() const;
    void setRefreshing(bool newRefreshing);

    TimeZone *systemTimeZone() const;
    void setSystemTimeZone(TimeZone *newSystemTimeZone);

public slots:
    TimeZone *findTimeZoneById(const QString &id) const;
    void refreshZones();
    void refreshSystemZone();

signals:
    void refreshingChanged();
    void systemTimeZoneChanged();

private:
    bool _refreshing;
    TimeZone *_systemTimeZone;
};

class TimeZoneSortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(TimeZoneModel *model READ model WRITE setModel NOTIFY modelChanged FINAL)

public:
    explicit TimeZoneSortFilterModel();
    TimeZoneModel *model() const;
    void setModel(TimeZoneModel *newModel);

signals:
    void modelChanged();

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    virtual bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

private:
    TimeZoneModel *_model = nullptr;
};

#endif // TIMEZONEMODEL_H
