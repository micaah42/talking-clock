#include "lightingdisplay.h"

#include <QLoggingCategory>
#include <QPainter>

#include "lighting.h"

namespace {
Q_LOGGING_CATEGORY(self, "lightingdisplay", QtInfoMsg)
}

LightingDisplay::LightingDisplay()
    : _lighting{nullptr}
    , _spacing{0}
    , _radius{0}
{}

LightingBase *LightingDisplay::lighting() const
{
    return _lighting;
}

void LightingDisplay::setLighting(LightingBase *newLighting)
{
    if (_lighting == newLighting)
        return;

    if (_lighting)
        disconnect(_lighting, &LightingBase::rendered, this, &LightingDisplay::triggerUpdate);

    _lighting = newLighting;
    emit lightingChanged();

    if (_lighting) {
        connect(_lighting, &LightingBase::rendered, this, &LightingDisplay::triggerUpdate);
        this->update();
    }
}

void LightingDisplay::paint(QPainter *painter)
{
    qCDebug(self) << "drawing pixels...";

    auto &pixels = _lighting->pixels();

    const double pixelSize = pixels.size();
    double pixelWidth = (this->width() - (pixelSize - 1) * _spacing) / pixelSize;
    double stepSize = pixelWidth + _spacing;
    painter->setPen({Qt::transparent, 0});

    for (int i = 0; i < pixels.size(); i++) {
        painter->setBrush(pixels[i]->color());
        painter->drawRoundedRect(QRectF{i * stepSize, 0, pixelWidth, this->height()}, _radius, _radius);
    }
}

void LightingDisplay::triggerUpdate()
{
    this->update();
}

qreal LightingDisplay::spacing() const
{
    return _spacing;
}

void LightingDisplay::setSpacing(qreal newSpacing)
{
    if (qFuzzyCompare(_spacing, newSpacing))
        return;

    _spacing = newSpacing;
    emit spacingChanged();
}

qreal LightingDisplay::radius() const
{
    return _radius;
}

void LightingDisplay::setRadius(qreal newRadius)
{
    if (qFuzzyCompare(_radius, newRadius))
        return;

    _radius = newRadius;
    emit radiusChanged();
}
