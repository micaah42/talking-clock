#include "lightingdisplay.h"

#include <QLoggingCategory>
#include <QPainter>

#include "lighting.h"

namespace {
Q_LOGGING_CATEGORY(self, "lightingdisplay")
}

LightingDisplay::LightingDisplay()
    : m_lighting{nullptr}
    , m_whiteRatio{0.25}
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
    qCInfo(self) << "drawing pixels...";

    auto &pixels = m_lighting->pixels();
    double xf = width() / (double) pixels.size();
    painter->setPen({Qt::transparent, 0});

    for (int i = 0; i < pixels.size(); i++) {
        QColor drawColor;
        QColor color = pixels[i]->color();
        drawColor.setGreenF((1 - m_whiteRatio) * color.greenF() + m_whiteRatio * color.alphaF());
        drawColor.setBlueF((1 - m_whiteRatio) * color.blueF() + m_whiteRatio * color.alphaF());
        drawColor.setRedF((1 - m_whiteRatio) * color.redF() + m_whiteRatio * color.alphaF());

        painter->setBrush(drawColor);
        painter->drawRect(QRectF{i * xf, 0, xf, height()});
    }
}

void LightingDisplay::triggerUpdate()
{
    this->update();
}

double LightingDisplay::whiteRatio() const
{
    return m_whiteRatio;
}

void LightingDisplay::setWhiteRatio(double newWhiteRatio)
{
    if (qFuzzyCompare(m_whiteRatio, newWhiteRatio))
        return;
    m_whiteRatio = newWhiteRatio;
    emit whiteRatioChanged();
}
