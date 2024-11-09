#ifndef PRETTYRANDOMLIGHT_H
#define PRETTYRANDOMLIGHT_H

#include <QColor>
#include <QObject>
#include <QTimer>

#include "lightmode.h"

class Lighting;

class PrettyRandomLight : public LightMode
{
    Q_OBJECT
    Q_PROPERTY(double speed READ speed WRITE setSpeed NOTIFY speedChanged FINAL)
public:
    explicit PrettyRandomLight(Lighting &lighting, QObject *parent = nullptr);

    double speed() const;
    void setSpeed(double newSpeed);

signals:
    void speedChanged();

protected:
    void onTimeout();

private:
    double m_speed;
    QColor m_a;
    QColor m_b;
    QTimer _timer;
    QList<QColor> _gradient;
    QVector<double> _values;

    // LightMode interface
public:
    virtual QString name() const override { return "Pretty Random"; };
    virtual void start() override { _timer.start(); };
    virtual void stop() override { _timer.stop(); };
};

#endif // PRETTYRANDOMLIGHT_H
