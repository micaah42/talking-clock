#ifndef SYSTEMLIGHT_H
#define SYSTEMLIGHT_H

#include <QDir>
#include <QFile>
#include <QObject>

#include "qlighting_global.h"
#include "setting.h"

class SystemLight : public QObject
{
    Q_OBJECT
    QLIGHTING_ELEMENT

    Q_PROPERTY(QString path READ path CONSTANT FINAL)
    Q_PROPERTY(QString id READ id CONSTANT FINAL)
    Q_PROPERTY(int min READ min WRITE setMin NOTIFY minChanged FINAL)
    Q_PROPERTY(int max READ max WRITE setMax NOTIFY maxChanged FINAL)
    Q_PROPERTY(double brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged FINAL)
    Q_PROPERTY(double stepSize READ stepSize WRITE setStepSize NOTIFY stepSizeChanged FINAL)

public:
    explicit SystemLight(const QString &path, QObject *parent = nullptr);

    QString path() const;

    int min() const;
    void setMin(int newMin);

    int max() const;
    void setMax(int newMax);

    double brightness() const;
    void setBrightness(double newBrightness);

    double stepSize() const;
    void setStepSize(double newStepSize);

    QString id() const;

signals:
    void minChanged();
    void maxChanged();
    void brightnessChanged();
    void stepSizeChanged();

protected:
    QString key(const QString &name);
    int readMaxBrightness();
    void applyBrightness();

private:
    QString _path;
    QDir _dir;
    QFile _brightnessFile;
    QString _id;
    Setting<int> _min;
    Setting<int> _max;
    Setting<double> _brightness;
    Setting<double> _stepSize;
    Setting<QString> _match;
};

class SystemLightManager : public QObject
{
    Q_OBJECT
    QLIGHTING_SINGLETON

    Q_PROPERTY(QList<SystemLight *> lights READ lights WRITE setLights NOTIFY lightsChanged FINAL)
    Q_PROPERTY(bool refreshing READ refreshing WRITE setRefreshing NOTIFY refreshingChanged FINAL)

public:
    SystemLightManager(QObject *parent = nullptr);

    QList<SystemLight *> lights() const;

    bool refreshing() const;
    void setRefreshing(bool newRefreshing);

public slots:
    void refresh();

signals:
    void lightsChanged();
    void refreshingChanged();

protected:
    void setLights(const QList<SystemLight *> &newLights);

private:
    QList<SystemLight *> _lights;
    bool _refreshing;
};

#endif // SYSTEMLIGHT_H
