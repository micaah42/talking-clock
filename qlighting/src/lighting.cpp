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

LightingBase::LightingBase(QObject *parent)
    : QObject{parent}
    , _brightness{1.}
    , _enabled{false}
    , _mode{nullptr}
    , _leds{300}
    , _gpioPin{18}

{
    _pixels.reserve(this->leds());
    for (int i = 0; i < this->leds(); i++)
        _pixels.append(new Pixel{this});

    _timer.callOnTimeout(this, &LightingBase::onTimeout);
    _timer.setTimerType(Qt::PreciseTimer);
    _timer.setInterval(50);
}

LightingBase::~LightingBase() {}

void LightingBase::renderPixels()
{
    emit rendered();
}

LightMode *LightingBase::mode() const
{
    return _mode;
}

void LightingBase::setMode(LightMode *newMode)
{
    if (_mode == newMode)
        return;

    if (_mode) {
        disconnect(_mode, &LightMode::updateReqested, this, &LightingBase::onTimeout);
    }

    _mode = newMode;
    emit modeChanged();

    if (_mode) {
        connect(_mode, &LightMode::updateReqested, this, &LightingBase::onTimeout);

        _mode->reset();
        _mode->render(0, _pixels);

        this->renderPixels();
    }
}

double LightingBase::brightness() const
{
    return _brightness;
}

void LightingBase::setBrightness(double newBrightness)
{
    if (qFuzzyCompare(_brightness, newBrightness))
        return;

    _brightness = newBrightness;
    emit brightnessChanged();

    if (_enabled) {
        this->renderPixels();
    }
}

bool LightingBase::enabled() const
{
    return _enabled;
}

void LightingBase::onTimeout()
{
    if (_mode == nullptr)
        return;

    _mode->render(_elapsedTimer.restart() / 1000., _pixels);
    this->renderPixels();
}

void LightingBase::setPixels(const QList<Pixel *> &newPixels)
{
    _pixels = newPixels;
}

void LightingBase::setEnabled(bool newEnabled)
{
    if (_enabled == newEnabled)
        return;

    _enabled = newEnabled;
    emit enabledChanged();

    this->renderPixels();

    if (_enabled) {
        _elapsedTimer.start();
        _timer.start();
    }

    else {
        _elapsedTimer.invalidate();
        _timer.stop();
    }
}

const QList<Pixel *> &LightingBase::pixels() const
{
    return _pixels;
}

int LightingBase::interval() const
{
    return _timer.interval();
}

void LightingBase::setInterval(int newInterval)
{
    if (_timer.interval() == newInterval)
        return;

    _timer.setInterval(newInterval);
    emit intervalChanged();
}

int LightingBase::leds() const
{
    return _leds;
}

void LightingBase::setLeds(int newLeds)
{
    if (_leds == newLeds)
        return;

    _leds = newLeds;
    emit ledsChanged();
}

LightingWs2811::LightingWs2811(QObject *parent)
    : LightingBase{parent}
{
    this->initialize();
}

LightingWs2811::~LightingWs2811()
{
    this->setBrightness(0);
    LightingWs2811::renderPixels();
}

void LightingWs2811::renderPixels()
{
    qCDebug(self) << "rendering ws2811 leds...";

    auto leds = _ws2811->channel[0].leds;

    if (!leds) {
        qCCritical(self) << "ws2811 not properly initialized!";
        this->setEnabled(false);
        return;
    }

    auto brightness = this->renderBrightness();
    _ws2811->channel[0].brightness = brightness;

    for (int i = 0; i < this->pixels().size(); i++) {
        auto pixel = this->pixels()[i];
        auto &ws2811_led = leds[i];

        if (!pixel->_dirty)
            continue;

        pixel->_dirty = false;

        auto newColor = pixel->color();
        auto r = newColor.red();
        auto g = newColor.green();
        auto b = newColor.blue();
        auto w = std::min({r, g, b});

        ws2811_led = ws2811_led & 0x00FFFFFF;
        ws2811_led = ws2811_led | (w << 24);

        ws2811_led = ws2811_led & 0xFF00FFFF;
        ws2811_led = ws2811_led | (r << 16);

        ws2811_led = ws2811_led & 0xFFFF00FF;
        ws2811_led = ws2811_led | (g << 8);

        ws2811_led = ws2811_led & 0xFFFFFF00;
        ws2811_led = ws2811_led | b;
    }

    ws2811_return_t ret = ws2811_render(_ws2811.get());

    if (ret != WS2811_SUCCESS) {
        qCCritical(self) << "ws2811_render failed:" << ws2811_get_return_t_str(ret);
        return;
    }

    emit rendered();
}

bool LightingWs2811::initialize()
{
    _ws2811.reset(new ws2811_t{
        .freq = WS2811_TARGET_FREQ,
        .dmanum = DMA,
        .channel
        = {{
               .gpionum = this->gpioPin(),
               .invert = 0,
               .count = this->leds(),
               .strip_type = SK6812_STRIP_GRBW,
               .brightness = this->renderBrightness(),
           },
           {
               .gpionum = 0,
               .invert = 0,
               .count = 0,
               .brightness = 0,
           }},
    });

    auto oldPixels = this->pixels();
    QList<Pixel *> pixels;
    pixels.reserve(this->leds());

    for (int i = 0; i < this->leds(); i++)
        pixels.append(new Pixel{this});

    this->setPixels(pixels);
    qDeleteAll(oldPixels);

    auto ret = ws2811_init(_ws2811.get());
    if (ret != WS2811_SUCCESS) {
        qCCritical(self) << "ws2811_init failed:" << ws2811_get_return_t_str(ret);
        return false;
    }

    return true;
}

uint8_t LightingWs2811::renderBrightness()
{
    return this->enabled() ? 255 * this->brightness() : 0;
}

int LightingBase::gpioPin() const
{
    return _gpioPin;
}

void LightingBase::setGpioPin(int newGpioPin)
{
    if (_gpioPin == newGpioPin)
        return;

    _gpioPin = newGpioPin;
    emit gpioPinChanged();
}

Lighting::Lighting(QObject *parent)
    : LightingWs2811{parent}
{}
