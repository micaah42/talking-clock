#include "pixel.h"

Pixel::Pixel(ws2811_led_t &ws2811_led, QObject *parent)
    : QObject{parent}
    , _ws2811_led{ws2811_led}
{}

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

    _color = newColor;
    emit colorChanged();

    this->_setWhite(_color.alpha());
    this->_setGreen(_color.green());
    this->_setBlue(_color.blue());
    this->_setRed(_color.red());
}
