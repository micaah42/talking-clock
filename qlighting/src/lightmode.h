#ifndef LIGHTMODE_H
#define LIGHTMODE_H

#include <QObject>
#include <QTimer>
#include <QVariant>

class Lighting;
class Pixel;

class LightMode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT FINAL)
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged FINAL)

public:
    explicit LightMode(Lighting &lighting);
    static QList<QColor> gradient(int steps, const QColor &a, const QColor &b);
    static QColor interpolate(double f, const QColor &a, const QColor &b);

    virtual QString name() const = 0;

    virtual void start() = 0;
    virtual void stop() = 0;

    bool active() const;
    void setActive(bool newActive);

signals:
    void activeChanged();

protected:
    QList<Pixel *> &_pixels;
    Lighting &_lighting;

private:
    bool m_active;
    QString m_name;
};

class AnimatedLightMode : public LightMode
{
    Q_OBJECT
    Q_PROPERTY(int animationInvterval READ animationInvterval WRITE setAnimationInvterval NOTIFY animationInvtervalChanged FINAL)

public:
    explicit AnimatedLightMode(Lighting &lighting);

    virtual void animate(double delta) = 0;

    virtual void start() { _timer.start(); };
    virtual void stop() { _timer.stop(); };

    int animationInvterval() const;
    void setAnimationInvterval(int newAnimationInvterval);

protected:
    QList<Pixel *> &_pixels;

signals:
    void animationInvtervalChanged();

private:
    void onTimeout();
    QTimer _timer;
    qint64 _lastTime;
};

#endif // LIGHTMODE_H
