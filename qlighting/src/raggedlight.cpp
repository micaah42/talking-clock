#include "raggedlight.h"

#include <QLoggingCategory>
#include <random>

#include "lighting.h"
#include "pixel.h"

namespace {
Q_LOGGING_CATEGORY(self, "ragged", QtInfoMsg)
}

RaggedLight::RaggedLight(Lighting &lighting)
    : AnimatedLightMode{lighting}
    , _multiplier{0.001}
    , _chunkSize{32}
    , _saturation{1}
    , _value{1}
{
    qCDebug(self) << this;
}

QString RaggedLight::name() const
{
    return "Ragged";
}

double RaggedLight::multiplier() const
{
    return _multiplier;
}

void RaggedLight::setMultiplier(double newMultiplier)
{
    if (qFuzzyCompare(_multiplier, newMultiplier))
        return;

    _multiplier = newMultiplier;
    emit multiplierChanged();
}

double RaggedLight::chunkSize() const
{
    return _chunkSize;
}

void RaggedLight::setChunkSize(double newChunkSize)
{
    if (qFuzzyCompare(_chunkSize, newChunkSize))
        return;

    _chunkSize = newChunkSize;
    emit chunkSizeChanged();
}

void RaggedLight::animate(double delta)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> positionFn(0, 1);
    static std::normal_distribution<> hueFn(0, 1);

    double pos = positionFn(gen);
    int from = (_pixels.size() - _chunkSize) * pos;
    int to = (_pixels.size() - _chunkSize) * pos + _chunkSize;

    double hue = 0;

    for (int i = from; i < to; i++)
        hue += _pixels[i]->color().hueF();

    hue /= (to - from);
    hue += _multiplier * hueFn(gen);
    hue = hue >= 1 ? hue - 1 : hue;
    hue = hue < 0 ? hue + 1 : hue;

    for (int i = from; i < to; i++) {
        QColor c = QColor::fromHsv(255 * hue, 255 * _saturation, 255 * _value);
        _pixels[i]->setColor(c);
    }

    _lighting.render();
}

double RaggedLight::saturation() const
{
    return _saturation;
}

void RaggedLight::setSaturation(double newSaturation)
{
    if (qFuzzyCompare(_saturation, newSaturation))
        return;
    _saturation = newSaturation;
    emit saturationChanged();
}

double RaggedLight::value() const
{
    return _value;
}

void RaggedLight::setValue(double newValue)
{
    if (qFuzzyCompare(_value, newValue))
        return;
    _value = newValue;
    emit valueChanged();
}
