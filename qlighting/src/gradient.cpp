#include "gradient.h"

#include <QLoggingCategory>
#include <QPainter>
#include <QPixmap>

namespace {
Q_LOGGING_CATEGORY(self, "gradient")
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

    for (auto const &variant : _gradientStops) {
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

    this->setGradientStops(gradientStops);
}
