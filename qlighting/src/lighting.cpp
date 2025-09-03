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

Lighting::Lighting(int ledCount, QObject *parent)
    : QObject{parent}
    , _ws2811{new ws2811_t{
          .freq = WS2811_TARGET_FREQ,
          .dmanum = DMA,
          .channel = {{
                          .gpionum = GPIO_PIN,
                          .invert = 0,
                          .count = ledCount,
                          .strip_type = SK6812_STRIP_GRBW,
                          .brightness = 255,
                      },
                      {
                          .gpionum = 0,
                          .invert = 0,
                          .count = 0,
                          .brightness = 0,
                      }},
      }}
    , _modeType{"Lighting/Mode", LightMode::TypeStatic}
    , _brightness{"Lighting/Brighness", 1.}
    , _enabled{"Lighting/Enabled", true}
    , _mode{nullptr}
{
    ws2811_return_t ret;

    if ((ret = ws2811_init(_ws2811.get())) != WS2811_SUCCESS) {
        qCCritical(self) << "ws2811_init failed:" << ws2811_get_return_t_str(ret);
    }

    auto leds = _ws2811->channel[0].leds;
    leds = leds ? leds : new ws2811_led_t[LED_COUNT];

    for (int i = 0; i < LED_COUNT; i++)
        _pixels.append(new Pixel{leds[i]});

    this->setModeType(_modeType);

    _timer.callOnTimeout(this, &Lighting::onTimeout);
    _timer.setTimerType(Qt::PreciseTimer);
    _timer.setInterval(75);
}

Lighting::~Lighting()
{
    _ws2811->channel[0].brightness = 255 * _brightness;
    this->renderPixels();
}

void Lighting::renderPixels()
{
    emit rendered();

    if (!_ws2811->channel[0].leds)
        return;

    ws2811_return_t ret = ws2811_render(_ws2811.get());
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

    if (_mode) {
        disconnect(_mode, &LightMode::updateReqested, this, &Lighting::onTimeout);
    }

    _mode = newMode;
    emit modeChanged();

    if (_mode) {
        connect(_mode, &LightMode::updateReqested, this, &Lighting::onTimeout);
        _modeType = newMode->type();
    }
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
        _ws2811->channel[0].brightness = 255 * _brightness;
        this->renderPixels();
    }
}

bool Lighting::enabled() const
{
    return _enabled;
}

void Lighting::onTimeout()
{
    _mode->render(_elapsedTimer.restart(), _pixels);
    this->renderPixels();
}

void Lighting::setEnabled(bool newEnabled)
{
    if (_enabled == newEnabled)
        return;

    _enabled = newEnabled;
    emit enabledChanged();

    _ws2811->channel[0].brightness = _enabled ? 255 * _brightness : 0;
    this->renderPixels();

    _enabled ? _timer.start() : _timer.stop();
}

const QList<Pixel *> &Lighting::pixels() const
{
    return _pixels;
}



void Lighting::setModeType(LightMode::Type type)
{
    auto it = std::find_if(_modes.begin(), _modes.end(), [this](const LightMode *m) { //
        return _modeType == m->type();
    });

    if (it != _modes.end())
        this->setMode(*it);
}

ListModelBase *Lighting::lightModes() const
{
    return LightMode::allModes();
}

int Lighting::interval() const
{
    return _timer.interval();
}

void Lighting::setInterval(int newInterval)
{
    if (_timer.interval() == newInterval)
        return;

    _timer.setInterval(newInterval);
    emit intervalChanged();
}
