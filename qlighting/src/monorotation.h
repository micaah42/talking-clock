#ifndef MONOROTATION_H
#define MONOROTATION_H

#include <QColor>
#include <QObject>

#include "lightmode.h"

class Lighting;
class QVariantAnimation;
class QSequentialAnimationGroup;

class MonoRotationLight : public LightMode
{
    Q_OBJECT
    Q_PROPERTY(QList<QColor> colors READ colors WRITE setColors NOTIFY colorsChanged FINAL)
    Q_PROPERTY(int duration READ duration WRITE setDuration NOTIFY durationChanged FINAL)
public:
    explicit MonoRotationLight(Lighting &lighting);
    virtual QString name() const override;
    virtual Type type() const override;

    virtual void start() override;
    virtual void stop() override;
    QList<QColor> colors() const;
    void setColors(const QList<QColor> &newColors);

    int duration() const;
    void setDuration(int newDuration);

signals:
    void colorsChanged();

    void durationChanged();

protected:
    void resetAnimation();
    QVariantAnimation *animation(const QColor &from, const QColor &to, int duration);

private:
    QVariantAnimation *_animation;
    QList<QColor> _colors;
    int _duration;
};

#endif // MONOROTATION_H
