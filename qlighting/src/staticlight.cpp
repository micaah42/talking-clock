#include "staticlight.h"

#include "lighting.h"
#include "pixel.h"

StaticLight::StaticLight(Lighting &lighting)
    : LightMode{lighting}
    , m_color{0xff, 0, 0}
{}

QString StaticLight::name() const
{
    return "Static";
}

LightMode::Type StaticLight::type() const
{
    return TypeStatic;
}

void StaticLight::apply()
{
    for (auto &pixel : _pixels) {
        pixel->setColor(m_color);
    }

    _lighting.render();
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
void StaticLight::start()
{
    apply();
};
