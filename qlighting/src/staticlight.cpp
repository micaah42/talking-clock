#include "staticlight.h"

#include "pixel.h"

StaticLight::StaticLight()
    : _color{"StaticLight/Color", Qt::blue}
{}

void StaticLight::render(double delta, QList<Pixel *> &pixels)
{
    for (auto pixel : std::as_const(pixels))
        pixel->setColor(_color);
}

QString StaticLight::name() const
{
    return "Static";
}

LightMode::Type StaticLight::type() const
{
    return TypeStatic;
}


QColor StaticLight::color() const
{
    return _color;
}

void StaticLight::setColor(const QColor &newColor)
{
    if (_color == newColor)
        return;

    _color = newColor;
    emit colorChanged();

    this->update();
}
