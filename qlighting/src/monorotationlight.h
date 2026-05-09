#ifndef MONOROTATIONLIGHT_H
#define MONOROTATIONLIGHT_H

#include <QColor>
#include <QObject>
#include <QVariantAnimation>

#include "gradient.h"
#include "lightmode.h"
#include "qlighting_global.h"

class QLIGHTING_EXPORT MonoRotationLight : public AnimatedLightMode
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(LightingGradient *gradient READ gradient WRITE setGradient NOTIFY gradientChanged FINAL)
    Q_PROPERTY(WaveForm waveForm READ waveForm WRITE setWaveForm NOTIFY waveFormChanged FINAL)

public:
    enum WaveForm { Invalid, Sine, Sawtooth, Triangle };
    Q_ENUM(WaveForm);

    explicit MonoRotationLight();

    virtual void animatedRender(QList<Pixel *> &pixels) override;
    virtual Type type() const override;

    LightingGradient *gradient() const;
    void setGradient(LightingGradient *newGradient);

    WaveForm waveForm() const;
    void setWaveForm(const WaveForm &newWaveForm);

signals:
    void gradientChanged();
    void waveFormChanged();

private:
    LightingGradient *_gradient = nullptr;

    WaveForm _waveForm;
    std::function<double(double)> _waveFormFct;
};

#endif // MONOROTATIONLIGHT_H
