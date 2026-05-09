#include "weatherreportsample.h"

WeatherReportNextHours::WeatherReportNextHours(QObject *parent)
    : QObject(parent)
    , _type(Next1Hours)
    , _symbolCode()
    , _precipitationAmount(0.0)
{
}

WeatherReportSample::WeatherReportSample(QObject *parent)
    : QObject(parent)
    , _airPressureAtSeaLevel(0.0)
    , _airTemperature(0.0)
    , _cloudAreaFraction(0.0)
    , _relativeHumidity(0.0)
    , _windFromDirection(0.0)
    , _windSpeed(0.0)
    , _next1Hours(nullptr)
    , _next6Hours(nullptr)
    , _next12Hours(nullptr)
{
}

double WeatherReportSample::airPressureAtSeaLevel() const
{
    return _airPressureAtSeaLevel;
}

void WeatherReportSample::setAirPressureAtSeaLevel(double newAirPressureAtSeaLevel)
{
    if (qFuzzyCompare(_airPressureAtSeaLevel, newAirPressureAtSeaLevel))
        return;
    _airPressureAtSeaLevel = newAirPressureAtSeaLevel;
    emit airPressureAtSeaLevelChanged();
}

double WeatherReportSample::airTemperature() const
{
    return _airTemperature;
}

void WeatherReportSample::setAirTemperature(double newAirTemperature)
{
    if (qFuzzyCompare(_airTemperature, newAirTemperature))
        return;
    _airTemperature = newAirTemperature;
    emit airTemperatureChanged();
}

double WeatherReportSample::cloudAreaFraction() const
{
    return _cloudAreaFraction;
}

void WeatherReportSample::setCloudAreaFraction(double newCloudAreaFraction)
{
    if (qFuzzyCompare(_cloudAreaFraction, newCloudAreaFraction))
        return;
    _cloudAreaFraction = newCloudAreaFraction;
    emit cloudAreaFractionChanged();
}

double WeatherReportSample::relativeHumidity() const
{
    return _relativeHumidity;
}

void WeatherReportSample::setRelativeHumidity(double newRelativeHumidity)
{
    if (qFuzzyCompare(_relativeHumidity, newRelativeHumidity))
        return;
    _relativeHumidity = newRelativeHumidity;
    emit relativeHumidityChanged();
}

double WeatherReportSample::windFromDirection() const
{
    return _windFromDirection;
}

void WeatherReportSample::setWindFromDirection(double newWindFromDirection)
{
    if (qFuzzyCompare(_windFromDirection, newWindFromDirection))
        return;
    _windFromDirection = newWindFromDirection;
    emit windFromDirectionChanged();
}

double WeatherReportSample::windSpeed() const
{
    return _windSpeed;
}

void WeatherReportSample::setWindSpeed(double newWindSpeed)
{
    if (qFuzzyCompare(_windSpeed, newWindSpeed))
        return;
    _windSpeed = newWindSpeed;
    emit windSpeedChanged();
}

WeatherReportNextHours::Type WeatherReportNextHours::type() const
{
    return _type;
}

void WeatherReportNextHours::setType(const Type &newType)
{
    if (_type == newType)
        return;
    _type = newType;
    emit typeChanged();
}

QString WeatherReportNextHours::symbolCode() const
{
    return _symbolCode;
}

void WeatherReportNextHours::setSymbolCode(const QString &newSymbolCode)
{
    if (_symbolCode == newSymbolCode)
        return;
    _symbolCode = newSymbolCode;
    emit symbolCodeChanged();
}

double WeatherReportNextHours::precipitationAmount() const
{
    return _precipitationAmount;
}

void WeatherReportNextHours::setPrecipitationAmount(double newPrecipitationAmount)
{
    if (qFuzzyCompare(_precipitationAmount, newPrecipitationAmount))
        return;
    _precipitationAmount = newPrecipitationAmount;
    emit precipitationAmountChanged();
}

WeatherReportNextHours *WeatherReportSample::next1Hours() const
{
    return _next1Hours;
}

void WeatherReportSample::setNext1Hours(WeatherReportNextHours *newNext1Hours)
{
    if (_next1Hours == newNext1Hours)
        return;
    _next1Hours = newNext1Hours;
    emit next1HoursChanged();
}

WeatherReportNextHours *WeatherReportSample::next6Hours() const
{
    return _next6Hours;
}

void WeatherReportSample::setNext6Hours(WeatherReportNextHours *newNext6Hours)
{
    if (_next6Hours == newNext6Hours)
        return;
    _next6Hours = newNext6Hours;
    emit next6HoursChanged();
}

WeatherReportNextHours *WeatherReportSample::next12Hours() const
{
    return _next12Hours;
}

void WeatherReportSample::setNext12Hours(WeatherReportNextHours *newNext12Hours)
{
    if (_next12Hours == newNext12Hours)
        return;
    _next12Hours = newNext12Hours;
    emit next12HoursChanged();
}

QString WeatherReportSample::timeString() const
{
    return _timeString;
}

void WeatherReportSample::setTimeString(const QString &newTimeString)
{
    if (_timeString == newTimeString)
        return;
    _timeString = newTimeString;
    emit timeStringChanged();
}

QDateTime WeatherReportSample::time() const
{
    return _time;
}

void WeatherReportSample::setTime(const QDateTime &newTime)
{
    if (_time == newTime)
        return;
    _time = newTime;
    emit timeChanged();
}
