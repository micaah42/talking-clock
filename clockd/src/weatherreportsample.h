#ifndef WEATHERREPORTSAMPLE_H
#define WEATHERREPORTSAMPLE_H

#include <QObject>
#include <QString>
#include <QQmlEngine>

class WeatherReportNextHours : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(Type type READ type WRITE setType NOTIFY typeChanged FINAL)
    Q_PROPERTY(QString symbolCode READ symbolCode WRITE setSymbolCode NOTIFY symbolCodeChanged FINAL)
    Q_PROPERTY(double precipitationAmount READ precipitationAmount WRITE setPrecipitationAmount NOTIFY precipitationAmountChanged FINAL)

public:
    enum Type {
        Next1Hours,
        Next6Hours,
        Next12Hours,
    };
    Q_ENUM(Type);

    explicit WeatherReportNextHours(QObject *parent = nullptr);

    Type type() const;
    QString symbolCode() const;
    double precipitationAmount() const;

signals:
    void typeChanged();
    void symbolCodeChanged();
    void precipitationAmountChanged();

protected:
    void setType(const Type &newType);
    void setSymbolCode(const QString &newSymbolCode);
    void setPrecipitationAmount(double newPrecipitationAmount);

private:
    friend class WeatherService;
    Type _type;
    QString _symbolCode;
    double _precipitationAmount;
};

class WeatherReportSample : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    /* instant */
    Q_PROPERTY(double airPressureAtSeaLevel READ airPressureAtSeaLevel NOTIFY airPressureAtSeaLevelChanged FINAL)
    Q_PROPERTY(double airTemperature READ airTemperature NOTIFY airTemperatureChanged FINAL)
    Q_PROPERTY(double cloudAreaFraction READ cloudAreaFraction NOTIFY cloudAreaFractionChanged FINAL)
    Q_PROPERTY(double relativeHumidity READ relativeHumidity NOTIFY relativeHumidityChanged FINAL)
    Q_PROPERTY(double windFromDirection READ windFromDirection NOTIFY windFromDirectionChanged FINAL)
    Q_PROPERTY(double windSpeed READ windSpeed NOTIFY windSpeedChanged FINAL)

    /* next xxx hours reports */
    Q_PROPERTY(WeatherReportNextHours *next1Hours READ next1Hours WRITE setNext1Hours NOTIFY next1HoursChanged FINAL)
    Q_PROPERTY(WeatherReportNextHours *next6Hours READ next6Hours WRITE setNext6Hours NOTIFY next6HoursChanged FINAL)
    Q_PROPERTY(WeatherReportNextHours *next12Hours READ next12Hours WRITE setNext12Hours NOTIFY next12HoursChanged FINAL)

    /* time */
    Q_PROPERTY(QString timeString READ timeString WRITE setTimeString NOTIFY timeStringChanged FINAL)
    Q_PROPERTY(QDateTime time READ time WRITE setTime NOTIFY timeChanged FINAL)

public:
    explicit WeatherReportSample(QObject *parent = nullptr);

    double airPressureAtSeaLevel() const;
    double airTemperature() const;
    double cloudAreaFraction() const;
    double relativeHumidity() const;
    double windFromDirection() const;
    double windSpeed() const;

    WeatherReportNextHours *next1Hours() const;
    void setNext1Hours(WeatherReportNextHours *newNext1Hours);

    WeatherReportNextHours *next6Hours() const;
    void setNext6Hours(WeatherReportNextHours *newNext6Hours);

    WeatherReportNextHours *next12Hours() const;
    void setNext12Hours(WeatherReportNextHours *newNext12Hours);

    QString timeString() const;
    void setTimeString(const QString &newTimeString);

    QDateTime time() const;
    void setTime(const QDateTime &newTime);

signals:
    void airPressureAtSeaLevelChanged();
    void airTemperatureChanged();
    void cloudAreaFractionChanged();
    void relativeHumidityChanged();
    void windFromDirectionChanged();
    void windSpeedChanged();

    void next1HoursChanged();
    void next6HoursChanged();
    void next12HoursChanged();

    void timeStringChanged();
    void timeChanged();

protected:
    void setAirPressureAtSeaLevel(double newAirPressureAtSeaLevel);
    void setAirTemperature(double newAirTemperature);
    void setCloudAreaFraction(double newCloudAreaFraction);
    void setRelativeHumidity(double newRelativeHumidity);
    void setWindFromDirection(double newWindFromDirection);
    void setWindSpeed(double newWindSpeed);

private:
    friend class WeatherService;
    double _airPressureAtSeaLevel;
    double _airTemperature;
    double _cloudAreaFraction;
    double _relativeHumidity;
    double _windFromDirection;
    double _windSpeed;
    
    WeatherReportNextHours *_next1Hours = nullptr;
    WeatherReportNextHours *_next6Hours = nullptr;
    WeatherReportNextHours *_next12Hours = nullptr;

    QString _timeString;
    QDateTime _time;
};

#endif // WEATHERREPORTSAMPLE_H
