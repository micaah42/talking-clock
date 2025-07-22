#ifndef LIGHTING_H
#define LIGHTING_H

#include <QObject>
#include <memory>

#include "setting.h"

#include "lightmode.h"
#include "pixel.h"

#include "monorotation.h"
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
    MonoRotationLight *monoRotationLight() const;

public slots:
    void setModeType(LightMode::Type type);

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
    Setting<LightMode::Type> _modeType;
    Setting<double> _brightness;
    Setting<bool> _enabled;

    LightMode *_mode;
    StaticLight *_staticLight;
    WavingLight *_wavingLight;
    MonoRotationLight *_monoRotationLight;
    QList<LightMode *> _modes;
};

#endif // LIGHTING_H
