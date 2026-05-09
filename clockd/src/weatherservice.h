#ifndef WEATHERSERVICE_H
#define WEATHERSERVICE_H

#include <QNetworkAccessManager>
#include <QObject>
#include <QQmlEngine>
#include <QTimer>
#include <QDateTime>
#include <QVariantMap>

class WeatherReportSample;
class WeatherReportNextHours;
Q_MOC_INCLUDE("weatherreportsample.h");

class WeatherService : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    /* location */
    //  Q_PROPERTY(QGeoCoordinate coordinate READ coordinate NOTIFY coordinateChanged FINAL)
    Q_PROPERTY(QString cityName READ cityName NOTIFY cityNameChanged FINAL)

    /* current weather [oboslete] */
    Q_PROPERTY(QString currentSymbol READ currentSymbol NOTIFY currentSymbolChanged FINAL)
    Q_PROPERTY(double temperature READ temperature NOTIFY temperatureChanged FINAL)
    Q_PROPERTY(double windspeed READ windspeed NOTIFY windspeedChanged FINAL)
    Q_PROPERTY(double pressure READ pressure NOTIFY pressureChanged FINAL)
    Q_PROPERTY(double humidity READ humidity NOTIFY humidityChanged FINAL)
    Q_PROPERTY(double precipitationAmount READ precipitationAmount NOTIFY precipitationAmountChanged FINAL)
    Q_PROPERTY(QDateTime timestamp READ timestamp NOTIFY timestampChanged FINAL)

    /* current weather [new api] */
    Q_PROPERTY(WeatherReportSample *current READ current NOTIFY currentChanged FINAL)
    Q_PROPERTY(WeatherReportSample *tomorrow READ tomorrow NOTIFY tomorrowChanged FINAL)
    Q_PROPERTY(QList<WeatherReportSample *> todaySamples READ todaySamples NOTIFY todaySamplesChanged FINAL)
    Q_PROPERTY(QList<WeatherReportSample *> tomorrowSamples READ tomorrowSamples NOTIFY tomorrowSamplesChanged FINAL)
    Q_PROPERTY(QList<WeatherReportSample *> samples READ samples NOTIFY samplesChanged FINAL)

    Q_PROPERTY(Status status READ status NOTIFY statusChanged FINAL)

public:
    struct Coordinate
    {
        double latitude;
        double longitude;
    };

    enum Status {
        LocationError,
        WeatherError,
        Fetching,
        Valid,
    };

    Q_ENUM(Status);

    explicit WeatherService(QObject *parent = nullptr);

    static QString yr2gm(const QString &yr);
    Coordinate coordinate() const;
    QString currentSymbol() const;
    QString cityName() const;

    double temperature() const;
    double windspeed() const;
    double pressure() const;
    double humidity() const;
    double precipitationAmount() const;

    QDateTime timestamp() const;
    Status status() const;

    WeatherReportSample *current() const;
    WeatherReportSample *tomorrow() const;
    QList<WeatherReportSample *> todaySamples() const;
    QList<WeatherReportSample *> tomorrowSamples() const;
    QList<WeatherReportSample *> samples() const;

public slots:
    static QVariantMap summarizeWeatherSamples(const QList<WeatherReportSample *> &samples);
    void fetchWeatherData();
    void fetchQGeoPosition();
    void fetchIPLocation();

protected:
    WeatherReportNextHours *parseNextHours(const QJsonObject &nextHoursObject, QObject *parent);
    void setSamples(const QList<WeatherReportSample *> &newSamples);
    void setTodaySamples(const QList<WeatherReportSample *> &newTodaySamples);
    void setTomorrowSamples(const QList<WeatherReportSample *> &newTomorrowSamples);
    void setTimestamp(const QDateTime &newTimestamp);
    void setStatus(const Status &newStatus);
    void setCurrent(WeatherReportSample *newCurrent);
    void setTomorrow(WeatherReportSample *newTomorrow);

signals:
    void coordinateChanged();
    void cityNameChanged();

    void currentSymbolChanged();
    void temperatureChanged();
    void windspeedChanged();
    void pressureChanged();
    void humidityChanged();
    void precipitationAmountChanged();

    void timestampChanged();
    void statusChanged();

    void currentChanged();
    void tomorrowChanged();
    void todaySamplesChanged();
    void tomorrowSamplesChanged();

    void samplesChanged();

protected:
    void setCurrentSymbol(const QString &newCurrentSymbol);
    void setCoordinate(const Coordinate &newCoordinate);
    void setCityName(const QString &newCityName);

    void setTemperature(double newTemperature);
    void setWindspeed(double newWindspeed);
    void setPressure(double newPressure);
    void setHumidity(double newHumidity);
    void setPrecipitationAmount(double newPrecipitationAmount);

private:
    QTimer _timer;
    QNetworkAccessManager _manager;

    Coordinate _coordinate;
    QString _cityName;

    QString _currentSymbol;
    double _temperature;
    double _windspeed;
    double _pressure;
    double _humidity;

    QDateTime _timestamp;
    Status _status;
    double _precipitationAmount;
    WeatherReportSample *_current = nullptr;
    WeatherReportSample *_tomorrow = nullptr;
    QList<WeatherReportSample *> _todaySamples;
    QList<WeatherReportSample *> _tomorrowSamples;
    QList<WeatherReportSample *> _samples;
};

#endif // WEATHERSERVICE_H
