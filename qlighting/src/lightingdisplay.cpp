#include "lightingdisplay.h"

#include <QLoggingCategory>
#include <QPainter>

#include "lighting.h"

namespace {
Q_LOGGING_CATEGORY(self, "lightingdisplay", QtInfoMsg)
}

LightingDisplay::LightingDisplay()
    : m_lighting{nullptr}

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
    double xf = width() / (double) pixels.size();
    painter->setPen({Qt::transparent, 0});

    for (int i = 0; i < pixels.size(); i++) {
        painter->setBrush(pixels[i]->color());
        painter->drawRect(QRectF{i * xf, 0, xf, height()});
    }
}

void LightingDisplay::triggerUpdate()
{
    this->update();
}
