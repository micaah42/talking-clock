#ifndef PERFORMANCECONFIGURATION_H
#define PERFORMANCECONFIGURATION_H

#include <QObject>
#include <QQmlEngine>

#include <setting.h>

class PerformanceConfiguration : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(int visible READ visible WRITE setVisible NOTIFY visibleChanged FINAL)
    Q_PROPERTY(double x READ x WRITE setX NOTIFY xChanged FINAL)
    Q_PROPERTY(double y READ y WRITE setY NOTIFY yChanged FINAL)

public:
    explicit PerformanceConfiguration(QObject *parent = nullptr);

    int visible() const;
    void setVisible(int newVisible);

    double x() const;
    void setX(double newX);

    double y() const;
    void setY(double newY);

signals:
    void visibleChanged();
    void xChanged();
    void yChanged();

private:
    Setting<int> _visible;
    Setting<double> _x;
    Setting<double> _y;
};

#endif // PERFORMANCECONFIGURATION_H
