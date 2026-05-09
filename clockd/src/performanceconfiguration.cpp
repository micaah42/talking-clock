#include "performanceconfiguration.h"

PerformanceConfiguration::PerformanceConfiguration(QObject *parent)
    : QObject{parent}
    , _visible{"Performance/Visible", false}
    , _x{"Performance/CoordX", 64}
    , _y{"Performance/CoordY", 64}
{}

int PerformanceConfiguration::visible() const
{
    return _visible;
}

void PerformanceConfiguration::setVisible(int newVisible)
{
    if (_visible == newVisible)
        return;
    _visible = newVisible;
    emit visibleChanged();
}

double PerformanceConfiguration::x() const
{
    return _x;
}

void PerformanceConfiguration::setX(double newX)
{
    if (qFuzzyCompare(_x, newX))
        return;
    _x = newX;
    emit xChanged();
}

double PerformanceConfiguration::y() const
{
    return _y;
}

void PerformanceConfiguration::setY(double newY)
{
    if (qFuzzyCompare(_y, newY))
        return;
    _y = newY;
    emit yChanged();
}
