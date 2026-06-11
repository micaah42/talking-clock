#include "perlinlight.h"

#include <cmath>

#include <QLoggingCategory>

#include "pixel.h"

namespace {
Q_LOGGING_CATEGORY(self, "perlinlight", QtInfoMsg)
}

PerlinLight::PerlinLight(QObject *parent)
    : AnimatedLightMode{}
    , _gradient{nullptr}
    , _stretch{0.025}
    , _persistence{0.8}
    , _frequency{1.0}
    , _octaves{5}
{
    this->setSpeed(0.5);
}

void PerlinLight::animatedRender(QList<Pixel *> &pixels)
{
    qCDebug(self) << "rendering...";

    if (!_gradient)
        return;

    for (int i = 0; i < pixels.size(); i++) {
        double y = 0;
        y += perlin_noise_1d_sum(this->t() + pixels.size() - i * _stretch);
        y += perlin_noise_1d_sum(this->t() + i * _stretch);
        y /= 2.;

        pixels[i]->setColor(_gradient->colorAt(y));
    }
}

LightMode::Type PerlinLight::type() const
{
    return TypePerlin;
}

double PerlinLight::stretch() const
{
    return _stretch;
}

void PerlinLight::setStretch(double newStretch)
{
    if (qFuzzyCompare(_stretch, newStretch))
        return;

    _stretch = newStretch;
    emit stretchChanged();
}

LightingGradient *PerlinLight::gradient()
{
    return _gradient;
}

void PerlinLight::setGradient(LightingGradient *newGradient)
{
    if (_gradient == newGradient)
        return;
    _gradient = newGradient;
    emit gradientChanged();
}

double PerlinLight::persistence() const
{
    return _persistence;
}

void PerlinLight::setPersistence(double newPersistence)
{
    if (qFuzzyCompare(_persistence, newPersistence))
        return;
    _persistence = newPersistence;
    emit persistenceChanged();
}

double PerlinLight::frequency() const
{
    return _frequency;
}

void PerlinLight::setFrequency(double newFrequency)
{
    if (qFuzzyCompare(_frequency, newFrequency))
        return;
    _frequency = newFrequency;
    emit frequencyChanged();
}

int PerlinLight::octaves() const
{
    return _octaves;
}

void PerlinLight::setOctaves(int newOctaves)
{
    if (_octaves == newOctaves)
        return;
    _octaves = newOctaves;
    emit octavesChanged();
}

float PerlinLight::mix(float x, float y, float a) const
{
    return a * y + (1 - a) * x;
}

float PerlinLight::hash(float i) const
{
    // In 1d, return a slope
    float h = i * 127.1;
    float t;
    float p = -1. + 2. * std::modf(sin(h) * 43758.1453123, &t);
    return p;
}

float PerlinLight::perlin_noise_1d(float d) const
{
    float i = floor(d);
    float f = d - i;

    // Smoothstep interpolation
    float y = f * f * f * (6. * f * f - 15. * f + 10.);

    float slope1 = hash(i);
    float slope2 = hash(i + 1.0);
    float v1 = f;
    float v2 = f - 1.0;

    float r = mix(slope1 * v1, slope2 * v2, y);
    r = r * 0.5 + 0.5; // Map to range 0 ~ 1
    return r;
}

float PerlinLight::perlin_noise_1d_sum(float x) const
{
    float amp = _persistence, freq = _frequency;
    float total = 0.0;
    for (int i = 0; i < _octaves; ++i) {
        total += perlin_noise_1d(x * freq) * amp;

        amp *= _persistence;
        freq *= 2.0;
    }

    return total;
}
