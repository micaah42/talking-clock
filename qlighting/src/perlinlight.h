#ifndef PERLINLIGHT_H
#define PERLINLIGHT_H

#include <QObject>

#include "gradient.h"
#include "lightmode.h"

class PerlinLight : public AnimatedLightMode
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(LightingGradient *gradient READ gradient WRITE setGradient NOTIFY gradientChanged FINAL)
    Q_PROPERTY(double stretch READ stretch WRITE setStretch NOTIFY stretchChanged FINAL)

public:
    explicit PerlinLight(QObject *parent = nullptr);

    virtual void animatedRender(QList<Pixel *> &pixels) override;
    virtual Type type() const override;

    double stretch() const;
    void setStretch(double newStretch);

    LightingGradient *gradient();
    void setGradient(LightingGradient *newGradient);

signals:
    void stretchChanged();
    void gradientChanged();

private:
    LightingGradient *_gradient;
    double _stretch;
};

#endif // PERLINLIGHT_H
