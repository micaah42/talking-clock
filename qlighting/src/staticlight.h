#ifndef STATICLIGHT_H
#define STATICLIGHT_H

#include <QObject>

#include "lightmode.h"

class Lighting;

class StaticLight : public LightMode
{
    Q_OBJECT
    Q_PROPERTY(double red READ red WRITE setRed NOTIFY redChanged FINAL)
    Q_PROPERTY(double green READ green WRITE setGreen NOTIFY greenChanged FINAL)
    Q_PROPERTY(double blue READ blue WRITE setBlue NOTIFY blueChanged FINAL)
    Q_PROPERTY(double white READ white WRITE setWhite NOTIFY whiteChanged FINAL)
public:
    explicit StaticLight(Lighting &lighting);

    double white() const;
    void setWhite(double newWhite);

    double red() const;
    void setRed(double newRed);

    double green() const;
    void setGreen(double newGreen);

    double blue() const;
    void setBlue(double newBlue);

signals:
    void redChanged();
    void greenChanged();
    void blueChanged();
    void whiteChanged();

protected:
    void apply();

private:
    double m_white;
    double m_red;
    double m_green;
    double m_blue;

    // LightMode interface
public:
    virtual QString name() const override { return "Static"; };
    virtual void start() override { apply(); };
    virtual void stop() override {};
};

#endif // STATICLIGHT_H