#ifndef MONOROTATION_H
#define MONOROTATION_H

#include <QColor>
#include <QObject>
#include <QVariantAnimation>

#include "lightmode.h"
#include "qlighting_global.h"

class MonoRotationLight : public LightMode
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QList<QColor> colors READ colors WRITE setColors NOTIFY colorsChanged FINAL)
    Q_PROPERTY(int duration READ duration WRITE setDuration NOTIFY durationChanged FINAL)
public:
    explicit MonoRotationLight();

    virtual void render(double delta, QList<Pixel *> &pixels) override;
    virtual QString name() const override;
    virtual Type type() const override;

    QList<QColor> colors() const;
    void setColors(const QList<QColor> &newColors);

    void setDuration(int newDuration);
    int duration() const;

signals:
    void durationChanged();
    void colorsChanged();

protected:
    QVariantAnimation *animation(const QColor &from, const QColor &to, int duration);
    void resetAnimation();

private:
    QScopedPointer<QVariantAnimation> _animation;
    QList<QColor> _colors;
    int _duration;
};

#endif // MONOROTATION_H
