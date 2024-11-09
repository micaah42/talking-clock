#include "spacetheme.h"

SpaceTheme::SpaceTheme(QObject *parent)
    : QObject{parent}
    , _settings{DEFAULT_SETTINGS_SCOPE}
    , _clockSize{"SpaceBackground/ClockSize", 1.4}
    , _spaceShip{"SpaceBackground/SpaceShip", false}
    , _animatedStars{"SpaceBackground/AnimatedStars", 20}
    , _stars{"SpaceBackground/Stars", 75}
    , _animationSpeed{"SpaceBackground/AnimationSpeed", 5000}
    , _fpsVisible{false}
{}

int SpaceTheme::animatedStars() const
{
    return _animatedStars;
}

void SpaceTheme::setAnimatedStars(int newAnimatedStars)
{
    if (_animatedStars == newAnimatedStars)
        return;

    _animatedStars = newAnimatedStars;
    emit animatedStarsChanged();
}

int SpaceTheme::stars() const
{
    return _stars;
}

void SpaceTheme::setStars(int newStars)
{
    if (_stars == newStars)
        return;

    _stars = newStars;
    emit starsChanged();
}

int SpaceTheme::animationSpeed() const
{
    return _animationSpeed;
}

void SpaceTheme::setAnimationSpeed(int newAnimationSpeed)
{
    if (_animationSpeed == newAnimationSpeed)
        return;

    _animationSpeed = newAnimationSpeed;
    emit animationSpeedChanged();
}

bool SpaceTheme::spaceShip() const
{
    return _spaceShip;
}

void SpaceTheme::setSpaceShip(bool newSpaceShip)
{
    if (_spaceShip == newSpaceShip)
        return;

    _spaceShip = newSpaceShip;
    emit spaceShipChanged();
}

double SpaceTheme::clockSize() const
{
    return _clockSize;
}

void SpaceTheme::setClockSize(double newClockSize)
{
    if (qFuzzyCompare(_clockSize, newClockSize))
        return;

    _clockSize = newClockSize;
    emit clockSizeChanged();
}

bool SpaceTheme::fpsVisible() const
{
    return _fpsVisible;
}

void SpaceTheme::setFpsVisible(bool newFpsVisible)
{
    if (_fpsVisible == newFpsVisible)
        return;
    _fpsVisible = newFpsVisible;
    emit fpsVisibleChanged();
}
