#include "staticlight.h"

#include "lighting.h"
#include "pixel.h"

StaticLight::StaticLight(Lighting &lighting)
    : LightMode{lighting}
    , m_white{0}
    , m_red{0}
    , m_green{0}
    , m_blue{0}

{}

double StaticLight::white() const
{
    return m_white;
}

void StaticLight::setWhite(double newWhite)
{
    if (qFuzzyCompare(m_white, newWhite))
        return;

    m_white = newWhite;
    emit whiteChanged();

    apply();
}

void StaticLight::apply()
{
    for (auto &pixel : _pixels) {
        pixel->setWhite(255 * m_white);
        pixel->setGreen(255 * m_green);
        pixel->setBlue(255 * m_blue);
        pixel->setRed(255 * m_red);
    }

    _lighting.render();
}

double StaticLight::red() const
{
    return m_red;
}

void StaticLight::setRed(double newRed)
{
    if (qFuzzyCompare(m_red, newRed))
        return;

    m_red = newRed;
    emit redChanged();

    apply();
}

double StaticLight::green() const
{
    return m_green;
}

void StaticLight::setGreen(double newGreen)
{
    if (qFuzzyCompare(m_green, newGreen))
        return;

    m_green = newGreen;
    emit greenChanged();

    apply();
}

double StaticLight::blue() const
{
    return m_blue;
}

void StaticLight::setBlue(double newBlue)
{
    if (qFuzzyCompare(m_blue, newBlue))
        return;

    m_blue = newBlue;
    emit blueChanged();

    apply();
}
