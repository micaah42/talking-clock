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

public:
    explicit StaticLight(Lighting &lighting);

    virtual QString name() const override { return "Static"; };

    virtual void start() override { apply(); };
    virtual void stop() override {};

    QColor color() const;
    void setColor(const QColor &newColor);

signals:
    void colorChanged();

protected:
    void apply();

private:
    QColor m_color;
};

#endif // STATICLIGHT_H
