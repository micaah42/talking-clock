#ifndef LIGHTINGDISPLAY_H
#define LIGHTINGDISPLAY_H

#include <QQuickPaintedItem>

class LightingBase;

class LightingDisplay : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(LightingBase *lighting READ lighting WRITE setLighting NOTIFY lightingChanged FINAL)
    Q_PROPERTY(qreal spacing READ spacing WRITE setSpacing NOTIFY spacingChanged FINAL)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged FINAL)

public:
    LightingDisplay();

    LightingBase *lighting() const;
    void setLighting(LightingBase *newLighting);

    qreal spacing() const;
    void setSpacing(qreal newSpacing);

    qreal radius() const;
    void setRadius(qreal newRadius);

public slots:
    void triggerUpdate();

signals:
    void lightingChanged();
    void spacingChanged();
    void radiusChanged();

protected:
    virtual void paint(QPainter *painter) override;

private:
    // todo: add "onLightingDestroyed" slot
    LightingBase *_lighting;
    qreal _spacing;
    qreal _radius;
};

#endif // LIGHTINGDISPLAY_H
