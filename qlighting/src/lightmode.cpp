#include "lightmode.h"

#include <QColor>
#include <QDateTime>

#include "lighting.h"

LightMode::LightMode(Lighting &lighting)
    : QObject{&lighting}
    , _pixels{lighting._pixels}
    , _lighting{lighting}
    , m_active{false}
{}

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

bool LightMode::active() const
{
    return m_active;
}

void LightMode::setActive(bool newActive)
{
    if (m_active == newActive)
        return;

    m_active = newActive;
    emit activeChanged();

    m_active ? start() : stop();
}

AnimatedLightMode::AnimatedLightMode(Lighting &lighting)
    : LightMode{lighting}
    , _pixels{LightMode::_pixels}
{
    _timer.callOnTimeout(this, &AnimatedLightMode::onTimeout);
    _timer.setTimerType(Qt::PreciseTimer);
    _timer.setInterval(50);
}

int AnimatedLightMode::animationInvterval() const
{
    return _timer.interval();
}

void AnimatedLightMode::setAnimationInvterval(int newAnimationInvterval)
{
    if (_timer.interval() == newAnimationInvterval)
        return;

    _timer.setInterval(newAnimationInvterval);
    emit animationInvtervalChanged();
}

void AnimatedLightMode::onTimeout()
{
    auto now = QDateTime::currentMSecsSinceEpoch();
    this->animate(now - _lastTime / 1000.);
    _lastTime = now;
}