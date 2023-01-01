#include "palette.h"

Palette::Palette(SettingsService *settings) : _settings(settings), _frontlight(1.), _backlight(1.)
{
    // generate palette with https://color.adobe.com/create/image, dark mood
    _baseColors = {{"background", "#0D0B0C"},
                   {"primary", "#38358C"},
                   {"darkprimary", "#1A1940"},
                   {"accent", "#D93030"},
                   {"darkaccent", "#731F1F"}};

    // create settings
    _settings->create("colors/frontlight", 100);
    _settings->registerCallback("colors/frontlight",
                                [this](const QVariant &value) { setFrontlight(value.toInt()); });

    _settings->create("colors/backlight", 100);
    _settings->registerCallback("colors/backlight",
                                [this](const QVariant &value) { setBacklight(value.toInt()); });
}

const QVariantMap &Palette::colors() const
{
    return _colors;
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
    updateColors();
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
    updateColors();
    emit backlightChanged();
}

void Palette::updateColors()
{
    for (const auto &key : _baseColors.keys()) {
        Q_ASSERT(_baseColors[key].isValid());
        if (key == "background")
            _colors[key] = _baseColors[key].lighter(_backlight).name(QColor::HexRgb);
        else
            _colors[key] = _baseColors[key].lighter(_frontlight).name(QColor::HexRgb);
    }
    emit colorsChanged();
}
