#include "lightmode.h"

#include <QColor>
#include <QDateTime>

LightMode::LightMode()
{
    LightMode::allModes()->append(this);
}

void LightMode::update()
{
    emit updateReqested(QPrivateSignal());
}

QList<QColor> LightMode::gradient(int steps, const QColor &a, const QColor &b)
{
    QList<QColor> colors;
    for (int i = 0; i < steps; ++i) {
        auto color = interpolate(i / (double) (steps - 1), a, b);
        colors.append(color);
    }
    return colors;
}

QColor LightMode::interpolate(double f, const QColor &a, const QColor &b)
{
    double f1 = 1 - f;
    auto red = f * a.redF() + f1 * b.redF();
    auto green = f * a.greenF() + f1 * b.greenF();
    auto blue = f * a.blueF() + f1 * b.blueF();
    auto white = f * a.alphaF() + f1 * b.alphaF();
    return QColor(255 * red, 255 * green, 255 * blue, 255 * white);
}

ListModel<LightMode *> *LightMode::allModes()
{
    static ListModel<LightMode *> lightModes;
    return &lightModes;
}

AnimatedLightMode::AnimatedLightMode()
    : _t{0}
    , _speed{1}
{}

void AnimatedLightMode::render(double delta, QList<Pixel *> &pixels)
{
    _t += _speed * delta;
    this->animatedRender(pixels);
}

double AnimatedLightMode::speed() const
{
    return _speed;
}

double AnimatedLightMode::t() const
{
    return _t;
}

void AnimatedLightMode::setSpeed(double newSpeed)
{
    if (qFuzzyCompare(_speed, newSpeed))
        return;

    _speed = newSpeed;
    emit speedChanged();
}
