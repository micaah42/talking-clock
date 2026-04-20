#include "lightingdisplay.h"

#include <QLoggingCategory>
#include <QPainter>

#include "lighting.h"

namespace {
Q_LOGGING_CATEGORY(self, "lightingdisplay", QtInfoMsg)
}

LightingDisplay::LightingDisplay()
    : m_lighting{nullptr}
    , _spacing{0}
    , _radius{0}
{}

Lighting *LightingDisplay::lighting() const
{
    return m_lighting;
}

void LightingDisplay::setLighting(Lighting *newLighting)
{
    if (m_lighting == newLighting)
        return;

    if (m_lighting)
        disconnect(m_lighting, &Lighting::rendered, this, &LightingDisplay::triggerUpdate);

    m_lighting = newLighting;
    emit lightingChanged();

    if (m_lighting)
        connect(m_lighting, &Lighting::rendered, this, &LightingDisplay::triggerUpdate);
}

void LightingDisplay::paint(QPainter *painter)
{
    qCDebug(self) << "drawing pixels...";

    auto &pixels = m_lighting->pixels();

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
