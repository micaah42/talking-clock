#include "wavinglight.h"

#include <QLoggingCategory>

#include "lighting.h"
#include "pixel.h"

namespace {
Q_LOGGING_CATEGORY(self, "waving", QtInfoMsg)
}

WavingLight::WavingLight(Lighting &lighting)
    : LightMode{lighting}
    , m_speed{1}
    , m_length{48}
    , m_a{0xff, 0, 0}
    , m_b{0, 0, 0xff}
{
    connect(&_timer, &QTimer::timeout, this, &WavingLight::onTimeout);
    _timer.setTimerType(Qt::PreciseTimer);
    _timer.setInterval(1000 / 30);
    setupGradient();

    qCInfo(self) << "created" << this;
}

double WavingLight::speed() const
{
    return m_speed;
}

void WavingLight::setSpeed(double newSpeed)
{
    if (qFuzzyCompare(m_speed, newSpeed))
        return;

    m_speed = newSpeed;
    emit speedChanged();
}

int WavingLight::length() const
{
    return m_length;
}

void WavingLight::setLength(int newLength)
{
    if (m_length == newLength)
        return;

    m_length = newLength;
    emit lengthChanged();

    setupGradient();
}

QColor WavingLight::a() const
{
    return m_a;
}

void WavingLight::setA(const QColor &newA)
{
    if (m_a == newA)
        return;

    m_a = newA;
    emit aChanged();

    setupGradient();
}

QColor WavingLight::b() const
{
    return m_b;
}

void WavingLight::setB(const QColor &newB)
{
    if (m_b == newB)
        return;

    m_b = newB;
    emit bChanged();

    setupGradient();
}

int WavingLight::interval() const
{
    return _timer.interval();
}

void WavingLight::setInterval(int newInterval)
{
    if (_timer.interval() == newInterval)
        return;

    _timer.setInterval(newInterval);
    emit intervalChanged();
}

void WavingLight::setupGradient()
{
    _colors = gradient(m_length, m_a, m_b);
}

void WavingLight::onTimeout()
{
    for (int i = 0; i < _pixels.size(); ++i) {
        int sourceIndex = (i + m_shift) % (2 * _colors.size() - 1);
        sourceIndex = sourceIndex < _colors.size() ? sourceIndex : 2 * _colors.size() - sourceIndex - 1;
        _pixels[i]->setColor(_colors[sourceIndex]);
    }

    _lighting.render();
    m_shift += m_speed;
}
