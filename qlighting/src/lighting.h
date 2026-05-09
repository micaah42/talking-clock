#ifndef LIGHTING_H
#define LIGHTING_H

#include <QElapsedTimer>
#include <QObject>
#include <memory>

#include "setting.h"

#include "qlighting_global.h"

#include "lightmode.h"
#include "pixel.h"

struct ws2811_t;

class QLIGHTING_EXPORT LightingBase : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged FINAL)
    Q_PROPERTY(double brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged FINAL)
    Q_PROPERTY(LightMode *mode READ mode WRITE setMode NOTIFY modeChanged FINAL)
    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged FINAL)

    Q_PROPERTY(int gpioPin READ gpioPin WRITE setGpioPin NOTIFY gpioPinChanged FINAL)
    Q_PROPERTY(int leds READ leds WRITE setLeds NOTIFY ledsChanged FINAL)

public:
    explicit LightingBase(QObject *parent = nullptr);
    ~LightingBase();

    LightMode *mode() const;
    void setMode(LightMode *newMode);

    double brightness() const;
    void setBrightness(double newBrightness);

    bool enabled() const;
    void setEnabled(bool newEnabled);

    const QList<Pixel *> &pixels() const;
    virtual void renderPixels();

    void setInterval(int newInterval);
    int interval() const;

    int leds() const;
    void setLeds(int newLeds);

    int gpioPin() const;
    void setGpioPin(int newGpioPin);

signals:
    void modeChanged();
    void brightnessChanged();
    void enabledChanged();
    void intervalChanged();
    void rendered();

    void ledsChanged();
    void gpioPinChanged();

protected:
    void setPixels(const QList<Pixel *> &newPixels);
    void onTimeout();

private:
    double _brightness;
    bool _enabled;
    QList<Pixel *> _pixels;

    QTimer _timer;
    QElapsedTimer _elapsedTimer;

    LightMode *_mode;
    int _leds;
    int _gpioPin;
};

class QLIGHTING_EXPORT LightingWs2811 : public LightingBase
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit LightingWs2811(QObject *parent = nullptr);
    ~LightingWs2811();

    virtual void renderPixels() override;

public slots:
    bool initialize();

private:
    uint8_t renderBrightness();
    std::unique_ptr<ws2811_t> _ws2811;
};

class QLIGHTING_EXPORT Lighting : public LightingWs2811
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit Lighting(QObject *parent = nullptr);
};

#endif // LIGHTING_H
