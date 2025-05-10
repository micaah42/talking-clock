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
    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged FINAL)
    Q_PROPERTY(double speed READ speed WRITE setSpeed NOTIFY speedChanged FINAL)

public:
    explicit AnimatedLightMode(Lighting &lighting);

    virtual void animate(double delta) = 0;

    virtual void start();
    virtual void stop();
    ;

    void setInterval(int newInterval);
    int interval() const;

    void setSpeed(double newSpeed);
    double speed() const;

protected:
    QList<Pixel *> &_pixels;

signals:
    void intervalChanged();
    void speedChanged();

private:
    void onTimeout();
    QTimer _timer;
    qint64 _lastTime;
    double _speed;
};

#endif // LIGHTMODE_H
