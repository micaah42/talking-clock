#include "palette.h"

Palette::Palette(SettingsService *settings)
    : _settings(settings)
    , _primary{0x38, 0x35, 0x8C}
    , _darkPrimary{0x1A, 0x19, 0x40}
    , _accent{0xD9, 0x30, 0x30}
    , _darkAccent{0x73, 0x1F, 0x1F}
    , _background{0x0D, 0x0B, 0x0C}
    , _frontlight(1.)
    , _backlight(1.)
{
    // generate palette with https://color.adobe.com/create/image, dark mood

    // create settings
    _settings->create("colors/frontlight", 100);
    _settings->registerCallback("colors/frontlight",
                                [this](const QVariant &value) { setFrontlight(value.toInt()); });

    _settings->create("colors/backlight", 100);
    _settings->registerCallback("colors/backlight",
                                [this](const QVariant &value) { setBacklight(value.toInt()); });
}

double Palette::frontlight() const
{
    return _frontlight;
}

void Palette::setFrontlight(double newFrontLight)
{
    if (_frontlight == newFrontLight)
        return;

    _frontlight = newFrontLight;
    emit frontlightChanged();
}

double Palette::backlight() const
{
    return _backlight;
}

void Palette::setBacklight(double newBacklight)
{
    if (_backlight == newBacklight)
        return;

    _backlight = newBacklight;
    emit backlightChanged();
}

QColor Palette::primary() const
{
    return _primary;
}

void Palette::setPrimary(const QColor &newPrimary)
{
    if (_primary == newPrimary)
        return;
    _primary = newPrimary;
    emit primaryChanged();
}

QColor Palette::darkPrimary() const
{
    return _darkPrimary;
}

void Palette::setDarkPrimary(const QColor &newDarkPrimary)
{
    if (_darkPrimary == newDarkPrimary)
        return;
    _darkPrimary = newDarkPrimary;
    emit darkPrimaryChanged();
}

QColor Palette::accent() const
{
    return _accent;
}

void Palette::setAccent(const QColor &newAccent)
{
    if (_accent == newAccent)
        return;
    _accent = newAccent;
    emit accentChanged();
}

QColor Palette::background() const
{
    return _background;
}

void Palette::setBackground(const QColor &newBackground)
{
    if (_background == newBackground)
        return;
    _background = newBackground;
    emit backgroundChanged();
}
