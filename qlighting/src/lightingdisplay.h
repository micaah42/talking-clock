#ifndef LIGHTINGDISPLAY_H
#define LIGHTINGDISPLAY_H

#include <QQuickPaintedItem>

class Lighting;

class LightingDisplay : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(double whiteRatio READ whiteRatio WRITE setWhiteRatio NOTIFY whiteRatioChanged FINAL)
    Q_PROPERTY(Lighting *lighting READ lighting WRITE setLighting NOTIFY lightingChanged FINAL)

public:
    LightingDisplay();

    Lighting *lighting() const;
    void setLighting(Lighting *newLighting);

    double whiteRatio() const;
    void setWhiteRatio(double newWhiteRatio);

public slots:
    void triggerUpdate();

signals:
    void whiteRatioChanged();
    void lightingChanged();

protected:
    virtual void paint(QPainter *painter) override;

private:
    Lighting *m_lighting;
    double m_whiteRatio;
};

#endif // LIGHTINGDISPLAY_H
