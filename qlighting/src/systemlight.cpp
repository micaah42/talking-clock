#include "systemlight.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QLoggingCategory>
#include <QProcess>

namespace {
Q_LOGGING_CATEGORY(self, "systemlight");
}

SystemLight::SystemLight(const QString &path, QObject *parent)
    : QObject{parent}
    , _path{path}
    , _dir{QFileInfo{path}.absolutePath()}
    , _brightnessFile{path}
    , _id{QString::number(qHash(_path), 36).left(4)}
    , _min{key("Min"), 0}
    , _max{key("Max"), readMaxBrightness()}
    , _brightness{key("Brightness"), 0.}
    , _stepSize{key("StepSize"), 1. / (double) (_max - _min)}
    , _match{key("Match"), _path}
{
    if (path != _match) {
        qCWarning(self) << "potetential collision detected:";
        qCWarning(self) << "-" << _match;
        qCWarning(self) << "-" << _path;
    }

    qCDebug(self) << this << _id << _path << _brightness;
    this->applyBrightness();
}

QList<SystemLight *> SystemLightManager::lights() const
{
    return _lights;
}

void SystemLightManager::refresh()
{
    if (_refreshing) {
        qCWarning(self) << "already refreshing!";
        return;
    }

    this->setRefreshing(true);
    auto process = new QProcess;
    process->setArguments({"/sys", "-name", "brightness"});
    process->setProgram("/usr/bin/find");

    connect(process, &QProcess::finished, this, [this, process]() {
        auto brightnessFiles = process->readAllStandardOutput().trimmed().split('\n');
        qCInfo(self) << "found" << brightnessFiles.size() << "files";
        process->deleteLater();

        QList<SystemLight *> lights;

        for (auto const &brightnessFile : std::as_const(brightnessFiles))
            lights.append(new SystemLight{brightnessFile, this});

        this->setLights(lights);
        this->setRefreshing(false);
    });

    process->start();
    if (!process->waitForStarted(50)) {
        qCWarning(self) << "failed to start process:" << process->errorString();
        this->setRefreshing(false);
        process->deleteLater();
    }
}

void SystemLightManager::setLights(const QList<SystemLight *> &newLights)
{
    if (_lights == newLights)
        return;

    for (auto light : std::as_const(_lights))
        light->deleteLater();

    _lights = newLights;
    emit lightsChanged();
}

QString SystemLight::path() const
{
    return _path;
}

int SystemLight::min() const
{
    return _min;
}

void SystemLight::setMin(int newMin)
{
    if (_min == newMin)
        return;

    _min = newMin;
    emit minChanged();
}

int SystemLight::max() const
{
    return _max;
}

void SystemLight::setMax(int newMax)
{
    if (_max == newMax)
        return;

    _max = newMax;
    emit maxChanged();
}

double SystemLight::brightness() const
{
    return _brightness;
}

void SystemLight::setBrightness(double newBrightness)
{
    if (qFuzzyCompare(_brightness, newBrightness))
        return;

    _brightness = newBrightness;
    emit brightnessChanged();

    this->applyBrightness();
}

QString SystemLight::key(const QString &name)
{
    return QString{"syslight-%1/%2"}.arg(_id, name);
}

int SystemLight::readMaxBrightness()
{
    QFile file{_dir.filePath("max_brightness")};

    if (!file.open(QIODevice::ReadOnly)) {
        qCWarning(self) << "failed to get max brightness" << file.fileName() << file.errorString();
        return -1;
    }

    QByteArray s = file.read(128);
    return s.toInt();
}

void SystemLight::applyBrightness()
{
    auto newBrightness = QByteArray::number((int) (_brightness * _max));
    qCInfo(self) << "setting brightness:" << newBrightness;

    if (!_brightnessFile.open(QIODevice::WriteOnly)) {
        qCWarning(self) << "failed to set brightness:" << _brightnessFile.fileName() << _brightnessFile.errorString();
        return;
    }

    _brightnessFile.write(newBrightness);
    _brightnessFile.close();
}

bool SystemLightManager::refreshing() const
{
    return _refreshing;
}

void SystemLightManager::setRefreshing(bool newRefreshing)
{
    if (_refreshing == newRefreshing)
        return;

    _refreshing = newRefreshing;
    emit refreshingChanged();
}

double SystemLight::stepSize() const
{
    return _stepSize;
}

void SystemLight::setStepSize(double newStepSize)
{
    if (qFuzzyCompare(_stepSize, newStepSize))
        return;

    _stepSize = newStepSize;
    emit stepSizeChanged();
}

SystemLightManager::SystemLightManager(QObject *parent)
    : QObject(parent)
    , _refreshing(false)
{
    this->refresh();
}

QString SystemLight::id() const
{
    return _id;
}
