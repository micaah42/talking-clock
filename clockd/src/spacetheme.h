#ifndef SPACETHEME_H
#define SPACETHEME_H

#include <QObject>

#include "setting.h"

class SpaceTheme : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double clockSize READ clockSize WRITE setClockSize NOTIFY clockSizeChanged FINAL)
    Q_PROPERTY(bool fpsVisible READ fpsVisible WRITE setFpsVisible NOTIFY fpsVisibleChanged FINAL)
    Q_PROPERTY(bool spaceShip READ spaceShip WRITE setSpaceShip NOTIFY spaceShipChanged FINAL)
    Q_PROPERTY(int animationSpeed READ animationSpeed WRITE setAnimationSpeed NOTIFY animationSpeedChanged FINAL)
    Q_PROPERTY(int animatedStars READ animatedStars WRITE setAnimatedStars NOTIFY animatedStarsChanged FINAL)
    Q_PROPERTY(int stars READ stars WRITE setStars NOTIFY starsChanged FINAL)
public:
    explicit SpaceTheme(QObject *parent = nullptr);

    int animatedStars() const;
    void setAnimatedStars(int newAnimatedStars);

    int stars() const;
    void setStars(int newStars);

    int animationSpeed() const;
    void setAnimationSpeed(int newAnimationSpeed);

    bool spaceShip() const;
    void setSpaceShip(bool newSpaceShip);

    double clockSize() const;
    void setClockSize(double newClockSize);

    bool fpsVisible() const;
    void setFpsVisible(bool newFpsVisible);

signals:
    void animatedStarsChanged();
    void starsChanged();
    void animationSpeedChanged();
    void spaceShipChanged();
    void clockSizeChanged();
    void fpsVisibleChanged();

private:
    Setting<int> _animatedStars;
    Setting<int> _stars;
    Setting<int> _animationSpeed;
    Setting<bool> _spaceShip;
    Setting<double> _clockSize;
    bool _fpsVisible;
};

#endif // SPACETHEME_H
