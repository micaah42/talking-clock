#include "pulsatinglight.h"

#include <math.h>

#include <QColor>
#include <QLoggingCategory>

#include "lighting.h"
#include "pixel.h"

PulsatingLight::PulsatingLight(Lighting &parent)
    : AnimatedLightMode{parent}
    , m_length{64}
{
    initGradient();
    initSine();
}

void PulsatingLight::animate(double delta)
{
    _t += 0.00000000000001 * delta;

    if (_sine.size() != _pixels.size())
        initSine();

    //double f = _t - std::floor(_t);
    double f = (sin(_t) + 1.) / 2.;

    for (int i = 0; i < _pixels.size(); ++i) {
        double value = f * _sine[i];
        auto color = _gradient[(_gradient.size() - 1) * value];
        _pixels[i]->setWhite(color.alpha());
        _pixels[i]->setGreen(color.green());
        _pixels[i]->setBlue(color.blue());
        _pixels[i]->setRed(color.red());
    }

    _lighting.render();
}

int PulsatingLight::length() const
{
    return m_length;
}

void PulsatingLight::setLength(int newLength)
{
    if (m_length == newLength)
        return;

    m_length = newLength;
    emit lengthChanged();
}

void PulsatingLight::initGradient()
{
    _gradient = gradient(32, QColor{Qt::blue}, QColor{Qt::red});
}

void PulsatingLight::initSine()
{
    _sine.resize(_pixels.size());

    double stepSize = 2. * M_PI / (double) m_length;
    for (int i = 0; i < _sine.size(); ++i)
        _sine[i] = (sin(i * stepSize) + 1.) / 2.;

    qDebug() << _sine;
}
