#include "wavinglight.h"

#include <QLoggingCategory>

#include "lighting.h"
#include "pixel.h"

namespace {
Q_LOGGING_CATEGORY(self, "waving", QtInfoMsg)
}

WavingLight::WavingLight(Lighting &lighting)
    : AnimatedLightMode{lighting}
    , _length{"WavingLight/Length", 30}
    , _a{"WavingLight/Color1", QColor{0xff, 0, 0}}
    , _b{"WavingLight/Color2", QColor{0, 0, 0xff}}
    , _t{0}
{
    qCInfo(self) << "created" << this;
}

LightMode::Type WavingLight::type() const
{
    return TypeWaving;
}

int WavingLight::length() const
{
    return _length;
}

void WavingLight::setLength(int newLength)
{
    if (_length == newLength)
        return;

    _length = newLength;
    emit lengthChanged();
}

QColor WavingLight::a() const
{
    return _a;
}

void WavingLight::setA(const QColor &newA)
{
    if (_a == newA)
        return;

    _a = newA;
    emit aChanged();
}

QColor WavingLight::b() const
{
    return _b;
}

void WavingLight::setB(const QColor &newB)
{
    if (_b == newB)
        return;

    _b = newB;
    emit bChanged();
}

void WavingLight::animate(double delta)
{
    _t += delta;
    double f = 2 * M_PI / _length;

    for (int i = 0; i < _pixels.size(); i++) {
        double x = i * f + _t;
        double y = (sin(x) + 1.) / 2.;
        _pixels[i]->setColor(interpolate(y, _a, _b));
    }

    _lighting.render();
}
QString WavingLight::name() const
{
    return "Waving";
};
