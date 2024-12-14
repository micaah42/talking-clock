#include "pixel.h"

Pixel::Pixel(ws2811_led_t &ws2811_led, QObject *parent)
    : QObject{parent}
    , _ws2811_led{ws2811_led}
{}

#if 0
void Pixel::setWhite(int v)
{
    _setWhite(v);
    _color.setAlpha(v);
    emit colorChanged();
}

void Pixel::setRed(int v)
{
    _setRed(v);
    _color.setRed(v);
    emit colorChanged();
}

void Pixel::setGreen(int v)
{
    _setGreen(v);
    _color.setGreen(v);
    emit colorChanged();
}

void Pixel::setBlue(int v)
{
    _setBlue(v);
    _color.setBlue(v);
    emit colorChanged();
}
#endif

void Pixel::_setWhite(int v)
{
    _ws2811_led = _ws2811_led & 0x00FFFFFF;
    _ws2811_led = _ws2811_led | (v << 24);
}

void Pixel::_setRed(int v)
{
    _ws2811_led = _ws2811_led & 0xFF00FFFF;
    _ws2811_led = _ws2811_led | (v << 16);
}

void Pixel::_setGreen(int v)
{
    _ws2811_led = _ws2811_led & 0xFFFF00FF;
    _ws2811_led = _ws2811_led | (v << 8);
}

void Pixel::_setBlue(int v)
{
    _ws2811_led = _ws2811_led & 0xFFFFFF00;
    _ws2811_led = _ws2811_led | v;
}

const ws2811_led_t &Pixel::ws2811_led() const
{
    return _ws2811_led;
}

QColor Pixel::color() const
{
    return _color;
}

void Pixel::setColor(const QColor &newColor)
{
    if (_color == newColor)
        return;

    auto r = newColor.red();
    auto g = newColor.green();
    auto b = newColor.blue();
    auto min = std::min({r, g, b});
    auto f = 255. / (255. - min + 1.);

    this->_setGreen(f * (g - min));
    this->_setBlue(f * (b - min));
    this->_setRed(f * (r - min));
    this->_setWhite(min);

    _color = newColor;
    emit colorChanged();
}
