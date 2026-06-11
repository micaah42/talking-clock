#include "perlinlight2d.h"

#include <cmath>

#include <QLoggingCategory>

#include "pixel.h"

namespace {
Q_LOGGING_CATEGORY(self, "perlinlight2d", QtInfoMsg)
}

PerlinLight2D::PerlinLight2D(QObject *parent)
    : AnimatedLightMode{}
    , _gradient{nullptr}
    , _stretchX{0.025}
    , _stretchT{0.025}
    , _persistence{0.8}
    , _frequency{1.0}
    , _octaves{5}
{
    this->setSpeed(0.5);
}

void PerlinLight2D::animatedRender(QList<Pixel *> &pixels)
{
    qCDebug(self) << "rendering 2d...";

    if (!_gradient)
        return;

    for (int i = 0; i < pixels.size(); i++) {
        double x = i * _stretchX;
        double t = this->t() * _stretchT;
        
        double y = perlin_noise_2d_sum(x, t);
        pixels[i]->setColor(_gradient->colorAt(y));
    }
}

LightMode::Type PerlinLight2D::type() const
{
    return TypePerlin;
}

double PerlinLight2D::stretchX() const
{
    return _stretchX;
}

void PerlinLight2D::setStretchX(double newStretchX)
{
    if (qFuzzyCompare(_stretchX, newStretchX))
        return;

    _stretchX = newStretchX;
    emit stretchXChanged();
}

double PerlinLight2D::stretchT() const
{
    return _stretchT;
}

void PerlinLight2D::setStretchT(double newStretchT)
{
    if (qFuzzyCompare(_stretchT, newStretchT))
        return;

    _stretchT = newStretchT;
    emit stretchTChanged();
}

LightingGradient *PerlinLight2D::gradient()
{
    return _gradient;
}

void PerlinLight2D::setGradient(LightingGradient *newGradient)
{
    if (_gradient == newGradient)
        return;
    _gradient = newGradient;
    emit gradientChanged();
}

double PerlinLight2D::persistence() const
{
    return _persistence;
}

void PerlinLight2D::setPersistence(double newPersistence)
{
    if (qFuzzyCompare(_persistence, newPersistence))
        return;
    _persistence = newPersistence;
    emit persistenceChanged();
}

double PerlinLight2D::frequency() const
{
    return _frequency;
}

void PerlinLight2D::setFrequency(double newFrequency)
{
    if (qFuzzyCompare(_frequency, newFrequency))
        return;
    _frequency = newFrequency;
    emit frequencyChanged();
}

int PerlinLight2D::octaves() const
{
    return _octaves;
}

void PerlinLight2D::setOctaves(int newOctaves)
{
    if (_octaves == newOctaves)
        return;
    _octaves = newOctaves;
    emit octavesChanged();
}

float PerlinLight2D::mix(float x, float y, float a) const
{
    return a * y + (1 - a) * x;
}

float PerlinLight2D::hash(float i, float j) const
{
    // 2D hash using combination of coordinates
    float h = i * 127.1 + j * 311.7;
    float t;
    float p = -1. + 2. * std::modf(std::sin(h) * 43758.1453123, &t);
    return p;
}

float PerlinLight2D::perlin_noise_2d(float x, float y) const
{
    float xi = std::floor(x);
    float yi = std::floor(y);
    float fx = x - xi;
    float fy = y - yi;

    // Smoothstep interpolation for both dimensions
    float u = fx * fx * fx * (6. * fx * fx - 15. * fx + 10.);
    float v = fy * fy * fy * (6. * fy * fy - 15. * fy + 10.);

    // Get hash values for all four corners
    float n00 = hash(xi, yi);
    float n10 = hash(xi + 1.0, yi);
    float n01 = hash(xi, yi + 1.0);
    float n11 = hash(xi + 1.0, yi + 1.0);

    // Gradient values
    float g00 = fx * n00 + fy * n00;
    float g10 = (fx - 1.0) * n10 + fy * n10;
    float g01 = fx * n01 + (fy - 1.0) * n01;
    float g11 = (fx - 1.0) * n11 + (fy - 1.0) * n11;

    // Interpolate along x
    float nx0 = mix(g00, g10, u);
    float nx1 = mix(g01, g11, u);

    // Interpolate along y
    float result = mix(nx0, nx1, v);
    result = result * 0.5 + 0.5; // Map to range 0 ~ 1

    return result;
}

float PerlinLight2D::perlin_noise_2d_sum(float x, float y) const
{
    float amp = _persistence;
    float freq = _frequency;
    float total = 0.0;

    for (int i = 0; i < _octaves; ++i) {
        total += perlin_noise_2d(x * freq, y * freq) * amp;

        amp *= _persistence;
        freq *= 2.0;
    }

    return total;
}
