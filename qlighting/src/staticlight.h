#ifndef STATICLIGHT_H
#define STATICLIGHT_H

#include <QColor>
#include <QObject>

#include "lightmode.h"

class Lighting;

class StaticLight : public LightMode
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)
    Q_PROPERTY(double white READ white WRITE setWhite NOTIFY whiteChanged FINAL)

    /* obsolete - use color! */
    Q_PROPERTY(double green READ green WRITE setGreen NOTIFY greenChanged FINAL)
    Q_PROPERTY(double blue READ blue WRITE setBlue NOTIFY blueChanged FINAL)
    Q_PROPERTY(double red READ red WRITE setRed NOTIFY redChanged FINAL)
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

    void colorChanged();

protected:
    void apply();

private:
    double m_white;
    QColor m_color;

    // LightMode interface
public:
    virtual QString name() const override { return "Static"; };
    virtual void start() override { apply(); };
    virtual void stop() override {};
    QColor color() const;
    void setColor(const QColor &newColor);
};

#endif // STATICLIGHT_H
