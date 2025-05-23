#include "timezonemodel.h"

#include <QLoggingCategory>
#include <QProcess>

namespace {
Q_LOGGING_CATEGORY(self, "timezone");
}

TimeZone::TimeZone(const QByteArray &id, QObject *parent)
    : QObject{parent}
    , _timeZone{id}
{
    _searchString = QStringList{
        _timeZone.id(),
        this->territory(),
        _timeZone.displayName(QDateTime::currentDateTime(), QTimeZone::ShortName),
        _timeZone.displayName(QDateTime::currentDateTime(), QTimeZone::LongName),
        _timeZone.displayName(QDateTime::currentDateTime(), QTimeZone::OffsetName),
    }.join(' ').toLower();
}

QString TimeZone::id() const
{
    return _timeZone.id();
}

QString TimeZone::territory() const
{
    return QLocale().territoryToString(_timeZone.territory());
}

QString TimeZone::comment() const
{
    return _timeZone.comment();
}

QString TimeZone::abbreviation(const QDateTime &atDateTime) const
{
    return _timeZone.abbreviation(atDateTime);
}

bool TimeZone::hasDaylightTime() const
{
    return _timeZone.hasDaylightTime();
}

bool TimeZone::hasTransitions() const
{
    return _timeZone.hasTransitions();
}

bool TimeZone::isValid() const
{
    return _timeZone.isValid();
}

const QString &TimeZone::searchString() const
{
    return _searchString;
}

QString TimeZone::displayName(const QDateTime &atDateTime, int nameType, const QLocale &locale) const
{
    return _timeZone.displayName(atDateTime, static_cast<QTimeZone::NameType>(nameType), locale);
}

int TimeZone::offsetFromUtc(const QDateTime &atDateTime) const
{
    return _timeZone.offsetFromUtc(atDateTime);
}

int TimeZone::standardTimeOffset(const QDateTime &atDateTime) const
{
    return _timeZone.standardTimeOffset(atDateTime);
}

int TimeZone::daylightTimeOffset(const QDateTime &atDateTime) const
{
    return _timeZone.daylightTimeOffset(atDateTime);
}

bool TimeZone::isDaylightTime(const QDateTime &atDateTime) const
{
    return _timeZone.isDaylightTime(atDateTime);
}

QDateTime TimeZone::convertDateTime(const QDateTime &dateTime)
{
    auto newDateTime = dateTime;
    newDateTime.setTimeZone(_timeZone);
    return newDateTime;
}

TimeZoneModel::TimeZoneModel(QObject *parent)
    : QListModel<TimeZone *>{parent}
    , _refreshing{false}
    , _systemTimeZone{nullptr}
{
    this->refreshZones();
}

void TimeZoneModel::refreshZones()
{
    if (_refreshing) {
        qCWarning(self) << "already refreshing!";
        return;
    }

    this->setRefreshing(true);
    auto process = new QProcess;
    process->setArguments({"/usr/share/zoneinfo", "-type", "f"});
    process->setProgram("/usr/bin/find");

    connect(process, &QProcess::finished, this, [this, process]() {
        auto timeZoneFiles = process->readAllStandardOutput().trimmed().split('\n');
        qCInfo(self) << "found" << timeZoneFiles.size() << "files";
        process->deleteLater();

        QList<TimeZone *> timeZones;

        for (auto const &timeZoneFile : std::as_const(timeZoneFiles)) {
            auto newTimeZone = new TimeZone{
                timeZoneFile.mid(QString{"/usr/share/zoneinfo"}.size() + 1),
                this,
            };

            if (!newTimeZone->isValid()) {
                newTimeZone->deleteLater();
                continue;
            }

            timeZones.append(newTimeZone);
        }

        this->clear();
        this->append(timeZones);
        this->setRefreshing(false);
        this->refreshSystemZone();
    });

    process->start();
    if (!process->waitForStarted(50)) {
        qCWarning(self) << "failed to start process:" << process->errorString();
        this->setRefreshing(false);
        process->deleteLater();
    }
}

void TimeZoneModel::refreshSystemZone()
{
    QProcess p;
    p.start("/usr/bin/timedatectl", {"show", "-p", "Timezone", "--value"});
    p.waitForFinished(250);
    auto id = p.readAllStandardOutput().trimmed();

    auto newSystemZone = this->findTimeZoneById(QString{id});
    if (newSystemZone == _systemTimeZone)
        return;

    _systemTimeZone = newSystemZone;
    emit systemTimeZoneChanged();

    qCInfo(self) << "refresh system timezone:" << id << newSystemZone;
}

bool TimeZoneModel::refreshing() const
{
    return _refreshing;
}

void TimeZoneModel::setRefreshing(bool newRefreshing)
{
    if (_refreshing == newRefreshing)
        return;
    _refreshing = newRefreshing;
    emit refreshingChanged();
}

TimeZoneSortFilterModel::TimeZoneSortFilterModel()
    : _model{nullptr}
{
    this->setFilterCaseSensitivity(Qt::CaseInsensitive);
}

TimeZoneModel *TimeZoneSortFilterModel::model() const
{
    return _model;
}

void TimeZoneSortFilterModel::setModel(TimeZoneModel *newModel)
{
    if (_model == newModel)
        return;

    _model = newModel;
    emit modelChanged();

    QSortFilterProxyModel::setSourceModel(_model);
    QSortFilterProxyModel::sort(0);
}

bool TimeZoneSortFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_ASSERT(_model);
    auto timeZone = _model->at(source_row);
    return this->filterRegularExpression().match(timeZone->searchString()).hasMatch();
}

bool TimeZoneSortFilterModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    auto a = _model->at(source_left.row());
    auto b = _model->at(source_right.row());
    auto now = QDateTime::currentDateTime();
    return a->offsetFromUtc(now) < b->offsetFromUtc(now);
}

TimeZone *TimeZoneModel::systemTimeZone() const
{
    return _systemTimeZone;
}

void TimeZoneModel::setSystemTimeZone(TimeZone *newSystemTimeZone)
{
    if (_systemTimeZone == newSystemTimeZone)
        return;

    _systemTimeZone = newSystemTimeZone;
    emit systemTimeZoneChanged();

    if (newSystemTimeZone == nullptr) {
        qCWarning(self) << "cannot set nullptr timezone!";
        return;
    }

    QProcess p;
    p.start("/usr/bin/timedatectl", {"set-timezone", newSystemTimeZone->id()});
    p.waitForFinished(50);
}

TimeZone *TimeZoneModel::findTimeZoneById(const QString &id) const
{
    auto it = std::find_if(this->cbegin(), this->cend(), [id](const TimeZone *timeZone) {
        return timeZone->id() == id;
    });
    return it == this->cend() ? nullptr : *it;
}
