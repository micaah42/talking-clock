#include "prettyrandomlight.h"

#include <QLoggingCategory>

#include "lighting.h"
#include "pixel.h"

PrettyRandomLight::PrettyRandomLight(Lighting &lighting, QObject *parent)
    : LightMode{lighting}
    , m_speed{0.1}
    , m_a{0xff, 0, 0, 0}
    , m_b{0, 0, 0xff, 0}
{
    connect(&_timer, &QTimer::timeout, this, &PrettyRandomLight::onTimeout);
    _timer.setTimerType(Qt::PreciseTimer);
    _timer.setInterval(1000 / 30);

    _gradient = gradient(32, m_a, m_b);
}

void PrettyRandomLight::onTimeout()
{
    if (_values.size() != _pixels.size())
        _values.resize(_pixels.size());

    for (int i = 0; i < _values.size(); i++) {
        double change = rand() / (double) RAND_MAX;
        _values[i] += m_speed * (2 * change - 1);
        _values[i] = std::clamp(_values[i], 0., 1.);
        auto color = _gradient[_values[i] * (_gradient.size() - 1)];
        _pixels[i]->setBlue(color.blue());
        _pixels[i]->setRed(color.red());
        _pixels[i]->setGreen(color.green());
        _pixels[i]->setWhite(color.alpha());
    }

    _lighting.render();
}

double PrettyRandomLight::speed() const
{
    return m_speed;
}

void PrettyRandomLight::setSpeed(double newSpeed)
{
    if (qFuzzyCompare(m_speed, newSpeed))
        return;

    m_speed = newSpeed;
    emit speedChanged();
}
