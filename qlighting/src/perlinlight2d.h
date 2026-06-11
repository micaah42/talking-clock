#ifndef PERLINLIGHT2D_H
#define PERLINLIGHT2D_H

#include <QObject>

#include "gradient.h"
#include "lightmode.h"

class PerlinLight2D : public AnimatedLightMode
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(LightingGradient *gradient READ gradient WRITE setGradient NOTIFY gradientChanged FINAL)
    Q_PROPERTY(double stretchX READ stretchX WRITE setStretchX NOTIFY stretchXChanged FINAL)
    Q_PROPERTY(double stretchT READ stretchT WRITE setStretchT NOTIFY stretchTChanged FINAL)
    Q_PROPERTY(double persistence READ persistence WRITE setPersistence NOTIFY persistenceChanged FINAL)
    Q_PROPERTY(double frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged FINAL)
    Q_PROPERTY(int octaves READ octaves WRITE setOctaves NOTIFY octavesChanged FINAL)

public:
    explicit PerlinLight2D(QObject *parent = nullptr);

    virtual void animatedRender(QList<Pixel *> &pixels) override;
    virtual Type type() const override;

    double stretchX() const;
    void setStretchX(double newStretchX);

    double stretchT() const;
    void setStretchT(double newStretchT);

    LightingGradient *gradient();
    void setGradient(LightingGradient *newGradient);

    double persistence() const;
    void setPersistence(double newPersistence);

    double frequency() const;
    void setFrequency(double newFrequency);

    int octaves() const;
    void setOctaves(int newOctaves);

signals:
    void stretchXChanged();
    void stretchTChanged();
    void gradientChanged();
    void persistenceChanged();
    void frequencyChanged();
    void octavesChanged();

private:
    float mix(float x, float y, float a) const;
    float hash(float i, float j) const;
    float perlin_noise_2d(float x, float y) const;
    float perlin_noise_2d_sum(float x, float y) const;

    LightingGradient *_gradient;
    double _stretchX;
    double _stretchT;
    double _persistence;
    double _frequency;
    int _octaves;
};

#endif // PERLINLIGHT2D_H
