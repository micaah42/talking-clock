#ifndef LIGHTMODE_H
#define LIGHTMODE_H

#include <QObject>
#include <QQmlEngine>
#include <QTimer>
#include <QVariant>

#include "listmodel.h"

class LightingBase;
class Pixel;

class LightMode : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Abstract")

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(Type type READ type CONSTANT FINAL)

public:
    enum Type {
        TypeStatic,
        TypeWaving,
        TypeMonoRotation,
        TypePerlin,
    };
    Q_ENUM(Type);

    static ListModel<LightMode *> *allModes();

    static QList<QColor> gradient(int steps, const QColor &a, const QColor &b);
    static QColor interpolate(double f, const QColor &a, const QColor &b);

    explicit LightMode();

    virtual void render(double delta, QList<Pixel *> &pixels) = 0;
    void update();

    virtual Type type() const = 0;
    virtual void reset() {};

    QString name() const;
    void setName(const QString &newName);

signals:
    void updateReqested(QPrivateSignal);
    void nameChanged();

private:
    bool _active;
    QString _name;
};

class AnimatedLightMode : public LightMode
{
    Q_OBJECT

    Q_PROPERTY(double speed READ speed WRITE setSpeed NOTIFY speedChanged FINAL)

public:
    explicit AnimatedLightMode();

    virtual void animatedRender(QList<Pixel *> &pixels) = 0;
    virtual void render(double delta, QList<Pixel *> &pixels);
    virtual void reset() { _t = 0; };

    void setSpeed(double newSpeed);
    double speed() const;

    double t() const;

signals:
    void speedChanged();

private:
    double _t;
    double _speed;
};

#endif // LIGHTMODE_H
