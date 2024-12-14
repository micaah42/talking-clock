#include "lighting.h"

#include "ws2811/ws2811.h"

#include <QLoggingCategory>

#define GPIO_PIN 18
#define DMA 10
#define WIDTH 300
#define HEIGHT 1
#define LED_COUNT (WIDTH * HEIGHT)

#include "pixel.h"
#include "prettyrandomlight.h"
#include "pulsatinglight.h"
#include "staticlight.h"
#include "wavinglight.h"

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
    , m_modes{new StaticLight{*this}, new WavingLight{*this}, new PrettyRandomLight{*this}, new PulsatingLight{*this}}
    , m_brightness{0.5}
    , m_enabled{true}
{
    ws2811_return_t ret;

    if ((ret = ws2811_init(&_d->ws2811)) != WS2811_SUCCESS) {
        qCCritical(self) << "ws2811_init failed:" << ws2811_get_return_t_str(ret);
    }

    auto leds = _d->ws2811.channel[0].leds;
    leds = leds ? leds : new ws2811_led_t[LED_COUNT];

    for (int i = 0; i < LED_COUNT; i++)
        _pixels.append(new Pixel{leds[i]});

    if (!m_modes.empty())
        this->setMode(m_modes.first());
}

void Lighting::render()
{
    emit rendered();

    ws2811_return_t ret;
    if (!_d->ws2811.channel[0].leds)
        return;

    if ((ret = ws2811_render(&_d->ws2811)) != WS2811_SUCCESS) {
        qCCritical(self) << "ws2811_render failed:" << ws2811_get_return_t_str(ret);
        return;
    }
}


LightMode *Lighting::mode() const
{
    return m_mode;
}

void Lighting::setMode(LightMode *newMode)
{
    if (m_mode == newMode)
        return;

    if (m_mode)
        m_mode->setActive(false);

    m_mode = newMode;
    emit modeChanged();

    if (m_mode && m_enabled)
        m_mode->setActive(true);
}

QList<LightMode *> Lighting::modes() const
{
    return m_modes;
}


double Lighting::brightness() const
{
    return m_brightness;
}

void Lighting::setBrightness(double newBrightness)
{
    if (qFuzzyCompare(m_brightness, newBrightness))
        return;

    m_brightness = newBrightness;
    emit brightnessChanged();

    _d->ws2811.channel[0].brightness = 255 * m_brightness;
    render();
}

bool Lighting::enabled() const
{
    return m_enabled;
}

void Lighting::setEnabled(bool newEnabled)
{
    if (m_enabled == newEnabled)
        return;

    m_enabled = newEnabled;
    emit enabledChanged();

    if (m_mode)
        m_mode->setActive(m_enabled);

    _d->ws2811.channel[0].brightness = m_enabled ? 255 * m_brightness : 0;
    render();
}

const QList<Pixel *> &Lighting::pixels() const
{
    return _pixels;
}

QList<Pixel *> &Lighting::pixels()
{
    return _pixels;
}
