#ifndef LIGHTINGDISPLAY_H
#define LIGHTINGDISPLAY_H

#include <QQuickPaintedItem>

class Lighting;

class LightingDisplay : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(Lighting *lighting READ lighting WRITE setLighting NOTIFY lightingChanged FINAL)

public:
    LightingDisplay();

    Lighting *lighting() const;
    void setLighting(Lighting *newLighting);

public slots:
    void triggerUpdate();

signals:
    void lightingChanged();

protected:
    virtual void paint(QPainter *painter) override;

private:
    Lighting *m_lighting;
};

#endif // LIGHTINGDISPLAY_H
