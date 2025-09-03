#ifndef STATICLIGHT_H
#define STATICLIGHT_H

#include <QColor>
#include <QObject>

#include <setting.h>

#include "lightmode.h"
#include "qlighting_global.h"

class Lighting;

class StaticLight : public LightMode
{
    Q_OBJECT
    QLIGHTING_SINGLETON

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)

public:
    explicit StaticLight();

    virtual void render(double delta, QList<Pixel *> &pixels) override;
    virtual QString name() const override;
    virtual Type type() const override;

    QColor color() const;
    void setColor(const QColor &newColor);

signals:
    void colorChanged();

private:
    Setting<QColor> _color;
};

#endif // STATICLIGHT_H
