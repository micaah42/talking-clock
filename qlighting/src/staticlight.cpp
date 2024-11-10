#include "staticlight.h"

#include "lighting.h"
#include "pixel.h"

StaticLight::StaticLight(Lighting &lighting)
    : LightMode{lighting}
    , m_white{0}
    , m_color{0xff, 0, 0}
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
        pixel->setGreen(m_color.green());
        pixel->setBlue(m_color.blue());
        pixel->setRed(m_color.red());
    }

    _lighting.render();
}

double StaticLight::red() const
{
    return m_color.red();
}

void StaticLight::setRed(double newRed)
{
    if (qFuzzyCompare((double) m_color.redF(), newRed))
        return;

    m_color.setRedF(newRed);
    emit redChanged();

    apply();
}

double StaticLight::green() const
{
    return m_color.green();
}

void StaticLight::setGreen(double newGreen)
{
    if (qFuzzyCompare((double) m_color.greenF(), newGreen))
        return;

    m_color.setGreenF(newGreen);
    emit greenChanged();

    apply();
}

double StaticLight::blue() const
{
    return m_color.blueF();
}

void StaticLight::setBlue(double newBlue)
{
    if (qFuzzyCompare((double) m_color.blueF(), newBlue))
        return;

    m_color.setBlueF(newBlue);
    emit blueChanged();

    apply();
}

QColor StaticLight::color() const
{
    return m_color;
}

void StaticLight::setColor(const QColor &newColor)
{
    if (m_color == newColor)
        return;

    m_color = newColor;
    emit colorChanged();

    apply();
}
