#ifndef WEATHERSERVICE_H
#define WEATHERSERVICE_H

//  #include <QGeoCoordinate>
#include <QNetworkAccessManager>
#include <QObject>
#include <QQmlEngine>
#include <QTimer>

class WeatherService : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    /* location */
    //  Q_PROPERTY(QGeoCoordinate coordinate READ coordinate NOTIFY coordinateChanged FINAL)
    Q_PROPERTY(QString cityName READ cityName NOTIFY cityNameChanged FINAL)

    /* current weather */
    Q_PROPERTY(QString currentSymbol READ currentSymbol NOTIFY currentSymbolChanged FINAL)
    Q_PROPERTY(double temperature READ temperature NOTIFY temperatureChanged FINAL)
    Q_PROPERTY(double windspeed READ windspeed NOTIFY windspeedChanged FINAL)
    Q_PROPERTY(double pressure READ pressure NOTIFY pressureChanged FINAL)
    Q_PROPERTY(double humidity READ humidity NOTIFY humidityChanged FINAL)
    Q_PROPERTY(double precipitationAmount READ precipitationAmount NOTIFY precipitationAmountChanged FINAL)

    Q_PROPERTY(QDateTime timestamp READ timestamp NOTIFY timestampChanged FINAL)
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

public slots:
    void fetchWeatherData();
    void fetchQGeoPosition();
    void fetchIPLocation();

protected:
    void setTimestamp(const QDateTime &newTimestamp);
    void setStatus(const Status &newStatus);

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
};

#endif // WEATHERSERVICE_H
