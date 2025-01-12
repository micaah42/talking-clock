#include "lighting.h"

#include "ws2811/ws2811.h"

#include <QLoggingCategory>

#define GPIO_PIN 18
#define DMA 10
#define WIDTH 300
#define HEIGHT 1
#define LED_COUNT (WIDTH * HEIGHT)

#include "pixel.h"

namespace {
Q_LOGGING_CATEGORY(self, "lighting", QtInfoMsg)
}

struct LightingPrivate
{
    ws2811_t ws2811 = {
        .freq = WS2811_TARGET_FREQ,
        .dmanum = DMA,
        .channel = {
            {
                .gpionum = GPIO_PIN,
                .invert = 0,
                .count = LED_COUNT,
                .strip_type = SK6812_STRIP_GRBW,
                .brightness = 255,
            },
            {
                .gpionum = 0,
                .invert = 0,
                .count = 0,
                .brightness = 0,
            },
    },};
};

Lighting::Lighting(QObject *parent)
    : QObject{parent}
    , _d{new LightingPrivate}
    , _brightness{0.5}
    , _enabled{true}
    , _staticLight{new StaticLight{*this}}
    , _wavingLight{new WavingLight{*this}}
    , _pulsatingLight{new PulsatingLight{*this}}
{
    ws2811_return_t ret;

    if ((ret = ws2811_init(&_d->ws2811)) != WS2811_SUCCESS) {
        qCCritical(self) << "ws2811_init failed:" << ws2811_get_return_t_str(ret);
    }

    auto leds = _d->ws2811.channel[0].leds;
    leds = leds ? leds : new ws2811_led_t[LED_COUNT];

    for (int i = 0; i < LED_COUNT; i++)
        _pixels.append(new Pixel{leds[i]});
}

void Lighting::render()
{
    emit rendered();

    if (!_d->ws2811.channel[0].leds)
        return;

    ws2811_return_t ret = ws2811_render(&_d->ws2811);

    if (ret != WS2811_SUCCESS) {
        qCCritical(self) << "ws2811_render failed:" << ws2811_get_return_t_str(ret);
        return;
    }
}


LightMode *Lighting::mode() const
{
    return _mode;
}

void Lighting::setMode(LightMode *newMode)
{
    if (_mode == newMode)
        return;

    if (_mode)
        _mode->setActive(false);

    _mode = newMode;
    emit modeChanged();

    if (_mode && _enabled)
        _mode->setActive(true);
}


double Lighting::brightness() const
{
    return _brightness;
}

void Lighting::setBrightness(double newBrightness)
{
    if (qFuzzyCompare(_brightness, newBrightness))
        return;

    _brightness = newBrightness;
    emit brightnessChanged();

    if (_enabled) {
        _d->ws2811.channel[0].brightness = 255 * _brightness;
        this->render();
    }
}

bool Lighting::enabled() const
{
    return _enabled;
}

void Lighting::setEnabled(bool newEnabled)
{
    if (_enabled == newEnabled)
        return;

    _enabled = newEnabled;
    emit enabledChanged();

    if (_mode)
        _mode->setActive(_enabled);

    _d->ws2811.channel[0].brightness = _enabled ? 255 * _brightness : 0;
    this->render();
}

const QList<Pixel *> &Lighting::pixels() const
{
    return _pixels;
}

QList<Pixel *> &Lighting::pixels()
{
    return _pixels;
}

StaticLight *Lighting::staticLight() const
{
    return _staticLight;
}

WavingLight *Lighting::wavingLight() const
{
    return _wavingLight;
}

PulsatingLight *Lighting::pulsatingLight() const
{
    return _pulsatingLight;
}
