#include "monorotation.h"

#include <QSequentialAnimationGroup>
#include <QVariantAnimation>

#include "lighting.h"
#include "pixel.h"

MonoRotationLight::MonoRotationLight(Lighting &lighting)
    : LightMode{lighting}
    , _animation{nullptr}
    , _duration{10000}
{
    _animation = new QVariantAnimation{this};
    _animation->setDuration(_duration * _colors.size());
    _animation->setLoopCount(-1);

    connect(_animation, &QVariantAnimation::valueChanged, this, [this]() {
        auto color = _animation->currentValue().value<QColor>();

        for (auto const pixel : std::as_const(_pixels))
            pixel->setColor(color);

        _lighting.render();
    });
}

QString MonoRotationLight::name() const
{
    return "Mono Rotation";
}

LightMode::Type MonoRotationLight::type() const
{
    return TypeMonoRotation;
}

void MonoRotationLight::start()
{
    _animation->start();
}

void MonoRotationLight::stop()
{
    _animation->stop();
}

QList<QColor> MonoRotationLight::colors() const
{
    return _colors;
}

void MonoRotationLight::setColors(const QList<QColor> &newColors)
{
    if (_colors == newColors)
        return;

    _colors = newColors;
    emit colorsChanged();

    _animation->setDuration(_duration * _colors.size());
    _animation->setKeyValues({});

    for (int i = 0; i < _colors.size(); i++)
        _animation->setKeyValueAt(i / (double) _colors.size(), _colors[i]);

    if (!_colors.empty())
        _animation->setEndValue(_colors.first());
}

QVariantAnimation *MonoRotationLight::animation(const QColor &from, const QColor &to, int duration)
{
    return new QVariantAnimation{this};
}

int MonoRotationLight::duration() const
{
    return _duration;
}

void MonoRotationLight::setDuration(int newDuration)
{
    if (_duration == newDuration)
        return;
    _duration = newDuration;
    emit durationChanged();

    _animation->setDuration(_duration * _colors.size());
}
