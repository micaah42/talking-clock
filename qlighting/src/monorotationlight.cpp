#include "monorotationlight.h"

#include <QLoggingCategory>

#include "pixel.h"

namespace {
Q_LOGGING_CATEGORY(self, "perlinlight", QtInfoMsg)
}
MonoRotationLight::MonoRotationLight()
    : AnimatedLightMode()
    , _waveForm{Invalid}
{
    this->setWaveForm(Sine);
}

void MonoRotationLight::animatedRender(QList<Pixel *> &pixels)
{
    if (!_gradient)
        return;

    double position = _waveFormFct(this->t());
    auto color = _gradient->colorAt(position);

    for (int i = 0; i < pixels.size(); i++)
        pixels[i]->setColor(color);
}

LightMode::Type MonoRotationLight::type() const
{
    return TypeMonoRotation;
}

LightingGradient *MonoRotationLight::gradient() const
{
    return _gradient;
}

void MonoRotationLight::setGradient(LightingGradient *newGradient)
{
    if (_gradient == newGradient)
        return;
    _gradient = newGradient;
    emit gradientChanged();
}

MonoRotationLight::WaveForm MonoRotationLight::waveForm() const
{
    return _waveForm;
}

void MonoRotationLight::setWaveForm(const WaveForm &newWaveForm)
{
    if (_waveForm == newWaveForm)
        return;

    _waveForm = newWaveForm;
    emit waveFormChanged();

    switch (_waveForm) {
        case Sine:
            _waveFormFct = [](double t) { return (std::sin(t * 2 * M_PI / 10.) + 1) / 2; };
            break;
        case Triangle:
            _waveFormFct = [](double t) {
                double x = std::fmod(t / 10., 1.0);
                return x < 0.5 ? 2 * x : 2 * (1 - x);
            };
            break;
        case Sawtooth:
            _waveFormFct = [](double t) { return std::fmod(t / 10., 1.0); };
            break;
        default:
            qCCritical(self) << "invalid waveform" << _waveForm;
        }
}
