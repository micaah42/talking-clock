#include "pulsatinglight.h"

#include <math.h>

#include <QColor>
#include <QLoggingCategory>

#include "lighting.h"
#include "pixel.h"

PulsatingLight::PulsatingLight(Lighting &parent)
    : AnimatedLightMode{parent}
    , m_length{64}
    , m_f1{0.1}
    , m_f2{0.1}
    , m_f3{0.1}
{
    initGradient();
}

void PulsatingLight::animate(double delta)
{
    _t += delta / 1e15;

    for (int i = 0; i < _pixels.size(); ++i) {
        double x = i / (double) _pixels.size();
        double value = (sin(m_f1 * M_PI * sin(x + m_f3 * _t) + m_f2 * x) + 1.) / 2.;
        auto color = _gradient[(_gradient.size() - 1) * value];
        _pixels[i]->setColor(color);
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
    _gradient = gradient(64, m_a, m_b);
}

double PulsatingLight::f1() const
{
    return m_f1;
}

void PulsatingLight::setF1(double newF1)
{
    if (qFuzzyCompare(m_f1, newF1))
        return;

    m_f1 = newF1;
    emit f1Changed();
}

double PulsatingLight::f2() const
{
    return m_f2;
}

void PulsatingLight::setF2(double newF2)
{
    if (qFuzzyCompare(m_f2, newF2))
        return;

    m_f2 = newF2;
    emit f2Changed();
}

QColor PulsatingLight::a() const
{
    return m_a;
}

void PulsatingLight::setA(const QColor &newA)
{
    if (m_a == newA)
        return;

    m_a = newA;
    emit aChanged();

    initGradient();
}

QColor PulsatingLight::b() const
{
    return m_b;
}

void PulsatingLight::setB(const QColor &newB)
{
    if (m_b == newB)
        return;

    m_b = newB;
    emit bChanged();

    initGradient();
}

double PulsatingLight::f3() const
{
    return m_f3;
}

void PulsatingLight::setF3(double newF3)
{
    if (qFuzzyCompare(m_f3, newF3))
        return;
    m_f3 = newF3;
    emit f3Changed();
}
