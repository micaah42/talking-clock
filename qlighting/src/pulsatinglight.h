#ifndef PULSATINGLIGHT_H
#define PULSATINGLIGHT_H

#include <QObject>

#include "lightmode.h"

class Lighting;
class Pixel;

class PulsatingLight : public AnimatedLightMode
{
    Q_OBJECT
    Q_PROPERTY(int length READ length WRITE setLength NOTIFY lengthChanged FINAL)
public:
    explicit PulsatingLight(Lighting &parent);
    virtual void animate(double delta) override;

    int length() const;
    void setLength(int newLength);

signals:
    void lengthChanged();

protected:
    void initGradient();
    void initSine();

private:
    QVector<double> _sine;
    QList<QColor> _gradient;
    double _t;
    int m_length;

public:
    virtual QString name() const override { return "Pulsating"; };
};

#endif // PULSATINGLIGHT_H
