#include "weatherservice.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>

#include <QLoggingCategory>
#include <algorithm>
#include <limits>

#include "weatherreportsample.h"

namespace {
Q_LOGGING_CATEGORY(self, "weather")
}

static const QHash<QString, QString> YR2MaterialSymbol{
    {"clearsky_day", "sunny"},
    {"clearsky_night", "clear_night"},
    {"clearsky_polartwilight", "wb_twilight"},
    {"fair_day", "wb_sunny"},
    {"fair_night", "nightlight_round"},
    {"fair_polartwilight", "wb_twilight"},
    {"partlycloudy_day", "partly_cloudy_day"},
    {"partlycloudy_night", "partly_cloudy_night"},
    {"partlycloudy_polartwilight", "partly_cloudy_day"},
    {"cloudy", "cloud"},
    {"rainshowers_day", "rainy_light"},
    {"rainshowers_night", "rainy_light"},
    {"rainshowers_polartwilight", "rainy_light"},
    {"rainshowersandthunder_day", "thunderstorm"},
    {"rainshowersandthunder_night", "thunderstorm"},
    {"rainshowersandthunder_polartwilight", "thunderstorm"},
    {"sleetshowers_day", "weather_mix"},
    {"sleetshowers_night", "weather_mix"},
    {"sleetshowers_polartwilight", "weather_mix"},
    {"snowshowers_day", "weather_snowy"},
    {"snowshowers_night", "weather_snowy"},
    {"snowshowers_polartwilight", "weather_snowy"},
    {"rain", "rainy"},
    {"heavyrain", "rainy_heavy"},
    {"heavyrainandthunder", "thunderstorm"},
    {"sleet", "weather_mix"},
    {"snow", "snowing"},
    {"snowandthunder", "thunderstorm"},
    {"fog", "foggy"},
    {"sleetshowersandthunder_day", "thunderstorm"},
    {"sleetshowersandthunder_night", "thunderstorm"},
    {"sleetshowersandthunder_polartwilight", "thunderstorm"},
    {"snowshowersandthunder_day", "thunderstorm"},
    {"snowshowersandthunder_night", "thunderstorm"},
    {"snowshowersandthunder_polartwilight", "thunderstorm"},
    {"rainandthunder", "thunderstorm"},
    {"sleetandthunder", "thunderstorm"},
    {"lightrainshowersandthunder_day", "thunderstorm"},
    {"lightrainshowersandthunder_night", "thunderstorm"},
    {"lightrainshowersandthunder_polartwilight", "thunderstorm"},
    {"heavyrainshowersandthunder_day", "thunderstorm"},
    {"heavyrainshowersandthunder_night", "thunderstorm"},
    {"heavyrainshowersandthunder_polartwilight", "thunderstorm"},
    {"lightssleetshowersandthunder_day", "thunderstorm"},
    {"lightssleetshowersandthunder_night", "thunderstorm"},
    {"lightssleetshowersandthunder_polartwilight", "thunderstorm"},
    {"heavysleetshowersandthunder_day", "thunderstorm"},
    {"heavysleetshowersandthunder_night", "thunderstorm"},
    {"heavysleetshowersandthunder_polartwilight", "thunderstorm"},
    {"lightssnowshowersandthunder_day", "thunderstorm"},
    {"lightssnowshowersandthunder_night", "thunderstorm"},
    {"lightssnowshowersandthunder_polartwilight", "thunderstorm"},
    {"heavysnowshowersandthunder_day", "thunderstorm"},
    {"heavysnowshowersandthunder_night", "thunderstorm"},
    {"heavysnowshowersandthunder_polartwilight", "thunderstorm"},
    {"lightrainandthunder", "thunderstorm"},
    {"lightsleetandthunder", "thunderstorm"},
    {"heavysleetandthunder", "thunderstorm"},
    {"lightsnowandthunder", "thunderstorm"},
    {"heavysnowandthunder", "thunderstorm"},
    {"lightrainshowers_day", "rainy_light"},
    {"lightrainshowers_night", "rainy_light"},
    {"lightrainshowers_polartwilight", "rainy_light"},
    {"heavyrainshowers_day", "rainy_heavy"},
    {"heavyrainshowers_night", "rainy_heavy"},
    {"heavyrainshowers_polartwilight", "rainy_heavy"},
    {"lightsleetshowers_day", "weather_mix"},
    {"lightsleetshowers_night", "weather_mix"},
    {"lightsleetshowers_polartwilight", "weather_mix"},
    {"heavysleetshowers_day", "weather_mix"},
    {"heavysleetshowers_night", "weather_mix"},
    {"heavysleetshowers_polartwilight", "weather_mix"},
    {"lightsnowshowers_day", "weather_snowy"},
    {"lightsnowshowers_night", "weather_snowy"},
    {"lightsnowshowers_polartwilight", "weather_snowy"},
    {"heavysnowshowers_day", "snowing"},
    {"heavysnowshowers_night", "snowing"},
    {"heavysnowshowers_polartwilight", "snowing"},
    {"lightrain", "rainy_light"},
    {"lightsleet", "weather_mix"},
    {"heavysleet", "weather_mix"},
    {"lightsnow", "weather_snowy"},
    {"heavysnow", "snowing"},
};

QString WeatherService::yr2gm(const QString &yr)
{
    auto it = YR2MaterialSymbol.find(yr);

    if (it != YR2MaterialSymbol.end()) {
        return *it;
    }

    qCCritical(self) << "no symbol found for" << yr;
    return "";
}

QVariantMap WeatherService::summarizeWeatherSamples(const QList<WeatherReportSample *> &samples)
{
    QVariantMap summary;
    summary["sampleCount"] = samples.count();

    if (samples.isEmpty()) {
        return summary;
    }

    QHash<QString, int> symbolCounts;
    QString mostCommonSymbol;
    int mostCommonSymbolCount = 0;

    double minTemperature = std::numeric_limits<double>::infinity();
    double maxTemperature = -std::numeric_limits<double>::infinity();
    double minPressure = std::numeric_limits<double>::infinity();
    double maxPressure = -std::numeric_limits<double>::infinity();
    double minHumidity = std::numeric_limits<double>::infinity();
    double maxHumidity = -std::numeric_limits<double>::infinity();
    double minWindSpeed = std::numeric_limits<double>::infinity();
    double maxWindSpeed = -std::numeric_limits<double>::infinity();
    double minCloudAreaFraction = std::numeric_limits<double>::infinity();
    double maxCloudAreaFraction = -std::numeric_limits<double>::infinity();
    double totalFuturePrecipitationAmount = 0.0;

    const auto countSymbolAndPrecipitation = [&](WeatherReportNextHours *next) {
        if (!next) {
            return;
        }

        const QString symbol = next->symbolCode();
        if (!symbol.isEmpty()) {
            const int count = ++symbolCounts[symbol];
            if (count > mostCommonSymbolCount) {
                mostCommonSymbolCount = count;
                mostCommonSymbol = symbol;
            }
        }

        totalFuturePrecipitationAmount += next->precipitationAmount();
    };

    for (auto sample : samples) {
        if (!sample) {
            continue;
        }

        const double temperature = sample->airTemperature();
        minTemperature = std::min(minTemperature, temperature);
        maxTemperature = std::max(maxTemperature, temperature);

        const double pressure = sample->airPressureAtSeaLevel();
        minPressure = std::min(minPressure, pressure);
        maxPressure = std::max(maxPressure, pressure);

        const double humidity = sample->relativeHumidity();
        minHumidity = std::min(minHumidity, humidity);
        maxHumidity = std::max(maxHumidity, humidity);

        const double windSpeed = sample->windSpeed();
        minWindSpeed = std::min(minWindSpeed, windSpeed);
        maxWindSpeed = std::max(maxWindSpeed, windSpeed);

        const double cloudAreaFraction = sample->cloudAreaFraction();
        minCloudAreaFraction = std::min(minCloudAreaFraction, cloudAreaFraction);
        maxCloudAreaFraction = std::max(maxCloudAreaFraction, cloudAreaFraction);

        countSymbolAndPrecipitation(sample->next1Hours());
        countSymbolAndPrecipitation(sample->next6Hours());
        countSymbolAndPrecipitation(sample->next12Hours());
    }

    summary["commonSymbol"] = mostCommonSymbol;
    summary["commonSymbolCount"] = mostCommonSymbolCount;
    summary["minTemperature"] = minTemperature;
    summary["maxTemperature"] = maxTemperature;
    summary["minPressure"] = minPressure;
    summary["maxPressure"] = maxPressure;
    summary["minHumidity"] = minHumidity;
    summary["maxHumidity"] = maxHumidity;
    summary["minWindSpeed"] = minWindSpeed;
    summary["maxWindSpeed"] = maxWindSpeed;
    summary["minCloudAreaFraction"] = minCloudAreaFraction;
    summary["maxCloudAreaFraction"] = maxCloudAreaFraction;
    summary["totalFuturePrecipitationAmount"] = totalFuturePrecipitationAmount;

    return summary;
}

WeatherReportNextHours *WeatherService::parseNextHours(const QJsonObject &nextHoursObject, QObject *parent)
{
    if (nextHoursObject.isEmpty()) {
        return nullptr;
    }

    auto nextHours = new WeatherReportNextHours{parent};
    auto summary = nextHoursObject["summary"].toObject();
    nextHours->setSymbolCode(yr2gm(summary["symbol_code"].toString()));
    auto details = nextHoursObject["details"].toObject();
    nextHours->setPrecipitationAmount(details["precipitation_amount"].toDouble());
    return nextHours;
};

void WeatherService::fetchWeatherData()
{
    if (_status == Fetching) {
        qCInfo(self) << "already fetching";
        return;
    }

    this->setStatus(Fetching);

    QUrl url("https://api.met.no/weatherapi/locationforecast/2.0/compact");
    url.setQuery({
        {"lat", QString::number(_coordinate.latitude)},
        {"lon", QString::number(_coordinate.longitude)},
    });

    QNetworkRequest request{url};
    request.setHeader(QNetworkRequest::UserAgentHeader, "talking-clock/1.0 (github.com/micaah42)");

    qCDebug(self) << "requesting weather:" << request.url();
    auto reply = _manager.get(request);

    QObject::connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            qCCritical(self) << "error fetching weather:" << reply->errorString();
            this->setStatus(WeatherError);
            reply->deleteLater();
            return;
        }

        auto obj = QJsonDocument::fromJson(reply->readAll()).object();

        const auto timeseries = obj["properties"].toObject()["timeseries"].toArray();
        QList<WeatherReportSample *> samples;
        QMap<QDateTime, WeatherReportSample *> sampleMap;
        QList<WeatherReportSample *> todaySamples;
        QList<WeatherReportSample *> tomorrowSamples;
        samples.reserve(timeseries.size());

        for (auto const element : timeseries) {
            auto dataPoint = element.toObject();
            auto data = dataPoint["data"].toObject();
            auto instant = data["instant"].toObject();
            auto instantDetails = instant["details"].toObject();

            auto newWeatherReportSample = new WeatherReportSample{this};

            newWeatherReportSample->setAirPressureAtSeaLevel(instantDetails["air_pressure_at_sea_level"].toDouble());
            newWeatherReportSample->setAirTemperature(instantDetails["air_temperature"].toDouble());
            newWeatherReportSample->setCloudAreaFraction(instantDetails["cloud_area_fraction"].toDouble());
            newWeatherReportSample->setRelativeHumidity(instantDetails["relative_humidity"].toDouble());
            newWeatherReportSample->setWindFromDirection(instantDetails["wind_from_direction"].toDouble());
            newWeatherReportSample->setWindSpeed(instantDetails["wind_speed"].toDouble());

            auto timeString = dataPoint["time"].toString();
            newWeatherReportSample->setTimeString(timeString);
            newWeatherReportSample->setTime(QDateTime::fromString(timeString, "yyyy-MM-ddThh:mm:ssZ"));

            auto next1Hours = parseNextHours(data["next_1_hours"].toObject(), this);
            if (next1Hours) {
                newWeatherReportSample->setNext1Hours(next1Hours);
            }
            auto next6Hours = parseNextHours(data["next_6_hours"].toObject(), this);
            if (next6Hours) {
                newWeatherReportSample->setNext6Hours(next6Hours);
            }
            auto next12Hours = parseNextHours(data["next_12_hours"].toObject(), this);
            if (next12Hours) {
                newWeatherReportSample->setNext12Hours(next12Hours);
            }

            sampleMap.insert(newWeatherReportSample->time(), newWeatherReportSample);
            samples.append(newWeatherReportSample);
        }

        std::sort(samples.begin(), samples.end(), [](WeatherReportSample *a, WeatherReportSample *b) {
            return a->time() < b->time();
        });

        const auto currentDate = QDate::currentDate();
        const auto tomorrowDate = currentDate.addDays(1);
        for (auto sample : samples) {
            if (sample->time().date() == currentDate)
                todaySamples.append(sample);
            else if (sample->time().date() == tomorrowDate)
                tomorrowSamples.append(sample);
        }

        auto now = QDateTime::currentDateTime();
        QDateTime currentTime{now.date(), {now.time().hour(), 0}};
        QDateTime tomorrowTime{tomorrowDate, {8, 0}};

        auto currentIt = sampleMap.find(currentTime);
        if (currentIt != sampleMap.end())
            this->setCurrent(*currentIt);
        else {
            qCWarning(self) << "no report timestamp found for" << currentTime;
            this->setCurrent(nullptr);
        }

        auto tomorrowIt = sampleMap.find(tomorrowTime);
        if (tomorrowIt != sampleMap.end())
            this->setTomorrow(*tomorrowIt);
        else {
            qCWarning(self) << "no report timestamp found for" << tomorrowTime;
            this->setTomorrow(nullptr);
        }

        this->setSamples(samples);
        this->setTodaySamples(todaySamples);
        this->setTomorrowSamples(tomorrowSamples);

        // old

        auto data0 =          //@
            obj["properties"] //@
                .toObject()["timeseries"]
                .toArray()
                .at(0)
                .toObject()["data"]
                .toObject();

        auto next1Hours = data0["next_1_hours"].toObject();

        // get status symbol icon

        auto currentCode =        //@
            next1Hours["summary"] //@
                .toObject()["symbol_code"]
                .toString();

        this->setCurrentSymbol(yr2gm(currentCode));

        // precipitation amount

        auto currentPrecipitationAmount = //@
            next1Hours["details"]         //@
                .toObject()["precipitation_amount"]
                .toDouble();

        this->setPrecipitationAmount(currentPrecipitationAmount);

        // instant details

        auto instantDetails = data0["instant"].toObject()["details"].toObject();

        this->setTemperature(instantDetails["air_temperature"].toDouble());
        this->setWindspeed(instantDetails["wind_speed"].toDouble());
        this->setPressure(instantDetails["air_pressure_at_sea_level"].toDouble());
        this->setHumidity(instantDetails["relative_humidity"].toDouble());

        this->setTimestamp(QDateTime::currentDateTime());
        this->setStatus(Valid);

        reply->deleteLater();
    });
}

void WeatherService::fetchQGeoPosition()
{
#if 0
    QGeoPositionInfoSource *source = QGeoPositionInfoSource::createDefaultSource(this);

    if (!source) {
        qCCritical(self) << "failed to create source!";
        return;
    }
    connect(source, &QGeoPositionInfoSource::positionUpdated, [this](const QGeoPositionInfo &info) {
        this->setCoordinate(info.coordinate());
    });
    connect(source, &QGeoPositionInfoSource::errorOccurred, [this](const QGeoPositionInfoSource::Error positioningError) {
        qCWarning(self) << "positioning error occurred:" << positioningError;
    });

    source->requestUpdate(10000);
#endif
}

void WeatherService::fetchIPLocation()
{
    QNetworkRequest request(QUrl{"http://ip-api.com/json/"});
    qCDebug(self) << "requesting ip location:" << request.url();
    QNetworkReply *reply = _manager.get(request);

    QObject::connect(reply, &QNetworkReply::finished, [reply, this]() {
        if (reply->error() != QNetworkReply::NoError) {
            qCCritical(self) << "Network Error:" << reply->errorString();
            reply->deleteLater();
            return;
        }

        auto obj = QJsonDocument::fromJson(reply->readAll()).object();

        qCInfo(self) << "received response:" << obj;

        if (obj["status"].toString() != "success") {
            qCCritical(self) << "invalid status:" << obj["status"];
            reply->deleteLater();
            return;
        }

        double lat = obj["lat"].toDouble();
        double lon = obj["lon"].toDouble();
        QString city = obj["city"].toString();

        qCDebug(self) << "location found (city, lat, lon):" << city << lat << lon;
        this->setCoordinate(Coordinate{lat, lon});
        this->setCityName(city);

        reply->deleteLater();
    });
}

WeatherService::WeatherService(QObject *parent)
    : QObject{parent}
    , _temperature{0}
    , _windspeed{0}
    , _pressure{0}
    , _humidity{0}
    , _precipitationAmount{0}
    , _status{LocationError}
{
    _timer.callOnTimeout(this, &WeatherService::fetchWeatherData);
    _timer.setInterval(std::chrono::hours{1});

    this->fetchIPLocation();
}

WeatherService::Coordinate WeatherService::coordinate() const
{
    return _coordinate;
}

void WeatherService::setCoordinate(const Coordinate &newCoordinate)
{
    if (_coordinate.latitude == newCoordinate.latitude && _coordinate.longitude == newCoordinate.longitude)
        return;

    qCInfo(self) << "setting coordinate:" << newCoordinate.latitude << newCoordinate.longitude;
    _coordinate = newCoordinate;
    emit coordinateChanged();

    this->fetchWeatherData();
    _timer.start();
}

QString WeatherService::currentSymbol() const
{
    return _currentSymbol;
}

void WeatherService::setCurrentSymbol(const QString &newCurrentSymbol)
{
    if (_currentSymbol == newCurrentSymbol)
        return;
    _currentSymbol = newCurrentSymbol;
    emit currentSymbolChanged();
}

QString WeatherService::cityName() const
{
    return _cityName;
}

void WeatherService::setCityName(const QString &newCityName)
{
    if (_cityName == newCityName)
        return;

    _cityName = newCityName;
    emit cityNameChanged();
}

double WeatherService::temperature() const
{
    return _temperature;
}

void WeatherService::setTemperature(double newTemperature)
{
    if (qFuzzyCompare(_temperature, newTemperature))
        return;

    _temperature = newTemperature;
    emit temperatureChanged();
}

double WeatherService::windspeed() const
{
    return _windspeed;
}

void WeatherService::setWindspeed(double newWindspeed)
{
    if (qFuzzyCompare(_windspeed, newWindspeed))
        return;
    _windspeed = newWindspeed;
    emit windspeedChanged();
}

double WeatherService::pressure() const
{
    return _pressure;
}

void WeatherService::setPressure(double newPressure)
{
    if (qFuzzyCompare(_pressure, newPressure))
        return;
    _pressure = newPressure;
    emit pressureChanged();
}

double WeatherService::humidity() const
{
    return _humidity;
}

void WeatherService::setHumidity(double newHumidity)
{
    if (qFuzzyCompare(_humidity, newHumidity))
        return;
    _humidity = newHumidity;
    emit humidityChanged();
}

QDateTime WeatherService::timestamp() const
{
    return _timestamp;
}

void WeatherService::setTimestamp(const QDateTime &newTimestamp)
{
    if (_timestamp == newTimestamp)
        return;
    _timestamp = newTimestamp;
    emit timestampChanged();
}

WeatherService::Status WeatherService::status() const
{
    return _status;
}

void WeatherService::setStatus(const Status &newStatus)
{
    if (_status == newStatus)
        return;

    qCInfo(self) << "set status:" << newStatus;
    _status = newStatus;
    emit statusChanged();
}

double WeatherService::precipitationAmount() const
{
    return _precipitationAmount;
}

void WeatherService::setPrecipitationAmount(double newPrecipitationAmount)
{
    if (qFuzzyCompare(_precipitationAmount, newPrecipitationAmount))
        return;

    _precipitationAmount = newPrecipitationAmount;
    emit precipitationAmountChanged();
}

WeatherReportSample *WeatherService::current() const
{
    return _current;
}

void WeatherService::setCurrent(WeatherReportSample *newCurrent)
{
    if (_current == newCurrent)
        return;
    _current = newCurrent;
    emit currentChanged();
}

WeatherReportSample *WeatherService::tomorrow() const
{
    return _tomorrow;
}

void WeatherService::setTomorrow(WeatherReportSample *newTomorrow)
{
    if (_tomorrow == newTomorrow)
        return;
    _tomorrow = newTomorrow;
    emit tomorrowChanged();
}

QList<WeatherReportSample *> WeatherService::samples() const
{
    return _samples;
}

void WeatherService::setSamples(const QList<WeatherReportSample *> &newSamples)
{
    if (_samples == newSamples)
        return;
    _samples = newSamples;
    emit samplesChanged();
}

QList<WeatherReportSample *> WeatherService::todaySamples() const
{
    return _todaySamples;
}

void WeatherService::setTodaySamples(const QList<WeatherReportSample *> &newTodaySamples)
{
    if (_todaySamples == newTodaySamples)
        return;
    _todaySamples = newTodaySamples;
    emit todaySamplesChanged();
}

QList<WeatherReportSample *> WeatherService::tomorrowSamples() const
{
    return _tomorrowSamples;
}

void WeatherService::setTomorrowSamples(const QList<WeatherReportSample *> &newTomorrowSamples)
{
    if (_tomorrowSamples == newTomorrowSamples)
        return;
    _tomorrowSamples = newTomorrowSamples;
    emit tomorrowSamplesChanged();
}
