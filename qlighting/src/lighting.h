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

class QLIGHTING_EXPORT Lighting : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged FINAL)
    Q_PROPERTY(double brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged FINAL)
    Q_PROPERTY(LightMode *mode READ mode WRITE setMode NOTIFY modeChanged FINAL)
    Q_PROPERTY(int modeIndex READ modeIndex WRITE setModeIndex NOTIFY modeIndexChanged FINAL)

    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged FINAL)

public:
    explicit Lighting(int ledCount = 300, QObject *parent = nullptr);
    ~Lighting();

    LightMode *mode() const;
    void setMode(LightMode *newMode);

    double brightness() const;
    void setBrightness(double newBrightness);

    bool enabled() const;
    void setEnabled(bool newEnabled);

    const QList<Pixel *> &pixels() const;
    void renderPixels();

    void setInterval(int newInterval);
    int interval() const;

    int modeIndex() const;
    void setModeIndex(int newModeIndex);

public slots:

signals:
    void modeChanged();
    void brightnessChanged();
    void enabledChanged();
    void intervalChanged();
    void rendered();
    void modeIndexChanged();

protected:
    void onTimeout();

private:
    std::unique_ptr<ws2811_t> _ws2811;
    Setting<double> _brightness;
    Setting<bool> _enabled;
    QList<Pixel *> _pixels;

    QTimer _timer;
    QElapsedTimer _elapsedTimer;

    LightMode *_mode;
    Setting<int> _modeIndex;
};

#endif // LIGHTING_H
