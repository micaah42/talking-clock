#ifndef LIGHTING_H
#define LIGHTING_H

#include <QObject>

#include "lightmode.h"
#include "pixel.h"

class Lighting : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged FINAL)
    Q_PROPERTY(LightMode *mode READ mode WRITE setMode NOTIFY modeChanged FINAL)
    Q_PROPERTY(double brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged FINAL)
    Q_PROPERTY(QList<LightMode *> modes READ modes CONSTANT FINAL)
    Q_PROPERTY(QList<Pixel *> pixels READ pixels CONSTANT FINAL)

public:
    explicit Lighting(QObject *parent = nullptr);

    void render();

    LightMode *mode() const;
    void setMode(LightMode *newMode);

    QList<LightMode *> modes() const;

    double brightness() const;
    void setBrightness(double newBrightness);

    bool enabled() const;
    void setEnabled(bool newEnabled);

    const QList<Pixel *> &pixels() const;
    QList<Pixel *> &pixels();

signals:
    void modeChanged();
    void brightnessChanged();
    void enabledChanged();
    void pixelsChanged();

protected:
    QList<Pixel *> _pixels;
    friend LightMode;

private:
    struct LightingPrivate *_d;
    QList<LightMode *> m_modes;
    LightMode *m_mode = nullptr;
    double m_brightness;
    bool m_enabled;
};

#endif // LIGHTING_H
