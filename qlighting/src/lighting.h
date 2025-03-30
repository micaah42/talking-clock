#ifndef LIGHTING_H
#define LIGHTING_H

#include <QObject>
#include <memory>

#include "lightmode.h"
#include "pixel.h"

#include "monorotation.h"
#include "pulsatinglight.h"
#include "staticlight.h"
#include "wavinglight.h"

struct ws2811_t;

class Lighting : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged FINAL)
    Q_PROPERTY(LightMode *mode READ mode WRITE setMode NOTIFY modeChanged FINAL)
    Q_PROPERTY(double brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged FINAL)
    Q_PROPERTY(StaticLight *staticLight READ staticLight CONSTANT FINAL)
    Q_PROPERTY(WavingLight *wavingLight READ wavingLight CONSTANT FINAL)
    Q_PROPERTY(PulsatingLight *pulsatingLight READ pulsatingLight CONSTANT FINAL)
    Q_PROPERTY(MonoRotationLight *monoRotationLight READ monoRotationLight CONSTANT FINAL)

public:
    explicit Lighting(int ledCount, QObject *parent = nullptr);
    ~Lighting();

    LightMode *mode() const;
    void setMode(LightMode *newMode);

    QList<LightMode *> modes() const;

    double brightness() const;
    void setBrightness(double newBrightness);

    bool enabled() const;
    void setEnabled(bool newEnabled);

    const QList<Pixel *> &pixels() const;
    QList<Pixel *> &pixels();
    void render();

    StaticLight *staticLight() const;
    WavingLight *wavingLight() const;
    PulsatingLight *pulsatingLight() const;
    MonoRotationLight *monoRotationLight() const;

signals:
    void modeChanged();
    void brightnessChanged();
    void enabledChanged();
    void rendered();

protected:
    QList<Pixel *> _pixels;
    friend LightMode;

private:
    std::unique_ptr<ws2811_t> _ws2811;
    LightMode *_mode = nullptr;
    double _brightness;
    bool _enabled;

    StaticLight *_staticLight;
    WavingLight *_wavingLight;
    PulsatingLight *_pulsatingLight;
    MonoRotationLight *_monoRotationLight;
};

#endif // LIGHTING_H
