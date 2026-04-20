#include "gradient.h"

#include <QLoggingCategory>
#include <QPainter>
#include <QPixmap>

namespace {
Q_LOGGING_CATEGORY(self, "gradient")
}

template<typename Enum>
QString enumName(Enum enumValue)
{
    static const auto metaEnum = QMetaEnum::fromType<Enum>();
    return QString(metaEnum.valueToKey(static_cast<int>(enumValue)));
}

template<typename Enum>
QString enumName(int enumValue)
{
    return enumName(static_cast<Enum>(enumValue));
}

LightingGradient::LightingGradient(QObject *parent)
    : QObject{parent}
    , _resolution{1024}
{}

QColor LightingGradient::colorAt(double value)
{
    int i = value * (double) (_resolution - 1.);
    return _colors[i];
}

void LightingGradient::updateColors()
{
    qCDebug(self) << "updating colors...";

    this->setFinalStop(_resolution - 1, 0);

    QImage image{_resolution, 1, QImage::Format_ARGB32_Premultiplied};
    QPainter painter{&image};

    QBrush brush{*this};
    painter.fillRect(0, 0, _resolution, 1, brush);

    _colors.resize(_resolution);

    for (int i = 0; i < _colors.size(); i++)
        _colors[i] = image.pixelColor(i, 0);
}

int LightingGradient::resolution() const
{
    return _resolution;
}

void LightingGradient::setResolution(int newResolution)
{
    if (_resolution == newResolution)
        return;

    _resolution = newResolution;
    emit resolutionChanged();

    this->updateColors();
}

QVariantList LightingGradient::gradientStops() const
{
    return _gradientStops;
}

void LightingGradient::setGradientStops(const QVariantList &newGradientStops)
{
    if (_gradientStops == newGradientStops)
        return;

    _gradientStops = newGradientStops;
    emit gradientStopsChanged();

    QGradientStops stops;

    for (auto const &variant : std::as_const(_gradientStops)) {
        auto variantMap = variant.toMap();
        stops.append(QGradientStop{
            variantMap["position"].toDouble(),
            variantMap["color"].toString(),
        });
    }

    this->setStops(stops);
    this->updateColors();
}

void LightingGradient::updateGradientStops()
{
    QVariantList gradientStops;

    for (auto const &stop : this->stops()) {
        gradientStops.append(QVariantMap{
            {"position", stop.first},
            {"color", stop.second},
        });
    }

    _gradientStops = gradientStops;
    emit gradientStopsChanged();
}

LightingGradient::Preset LightingGradient::preset() const
{
    return _preset;
}

void LightingGradient::setPreset(const Preset &newPreset)
{
    if (_preset == newPreset)
        return;

    _preset = newPreset;
    emit presetChanged();

    QGradient g{static_cast<QGradient::Preset>(newPreset)};
    this->setStops(g.stops());
    this->updateGradientStops();
    this->updateColors();
}

QString LightingGradient::presetName(int preset)
{
    return enumName<Preset>(preset);
}
