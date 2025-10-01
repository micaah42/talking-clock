#include "perlinlight.h"

#include <cmath>

#include <QLoggingCategory>

#include "pixel.h"

namespace {
Q_LOGGING_CATEGORY(self, "perlinlight", QtInfoMsg)
}

template<typename T>
int sign(T val)
{
    return (T(0) < val) - (val < T(0));
}

float mix(float x, float y, float a)
{
    return a * y + (1 - a) * x;
}

float hash(float i)
{
    //%in 1d, return a slope
    float h = i * 127.1;
    float t;
    float p = -1. + 2. * std::modf(sin(h) * 43758.1453123, &t);
    return p; //sin(p*6.283 + iTime);
}
float perlin_noise_1d(float d)
{
    float i = floor(d);
    float f = d - i;

    //%interpolation
    //float y = 6.0 * pow(f, 5.0) - 15.0 * pow(f, 4.0) + 10.0 * pow(f, 3.0);
    float y = f * f * f * (6. * f * f - 15. * f + 10.);

    float slope1 = hash(i);
    float slope2 = hash(i + 1.0);
    float v1 = f;
    float v2 = f - 1.0;

    float r = mix(slope1 * v1, slope2 * v2, y);
    r = r * 0.5 + 0.5; //%map to range 0 ~ 1
    return r;
}

float perlin_noise_1d_sum(float x)
{
    const float persistence = 0.8;
    const float frequence = 1.0;
    float amp = persistence, freq = frequence;
    float total = 0.0;
    for (int i = 0; i < 5; ++i) {
        total += perlin_noise_1d(x * freq) * amp;

        amp *= persistence;
        freq *= 2.0;
    }

    return total;
}

PerlinLight::PerlinLight(QObject *parent)
    : AnimatedLightMode{}
    , _stretch{0.025}
{
    _gradient.setColorAt(0.2, Qt::red);
    _gradient.setColorAt(0.5, Qt::yellow);
    _gradient.setColorAt(0.8, Qt::blue);
    _gradient.updateGradientStops();
    _gradient.updateColors();

    this->setSpeed(0.5);
}

void PerlinLight::animatedRender(QList<Pixel *> &pixels)
{
    qCDebug(self) << "rendering...";

    for (int i = 0; i < pixels.size(); i++) {
        double y = 0;
        y += perlin_noise_1d(this->t() + pixels.size() - i * _stretch);
        y += perlin_noise_1d(this->t() + i * _stretch);
        y /= 2.;

        pixels[i]->setColor(_gradient.colorAt(y));
    }
}

QString PerlinLight::name() const
{
    return "Perlin";
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
    return &_gradient;
}
