#ifndef GRADIENT_H
#define GRADIENT_H

#include <QGradient>
#include <QObject>
#include <QQmlEngine>

class LightingGradient : public QObject, public QLinearGradient
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QVariantList gradientStops READ gradientStops WRITE setGradientStops NOTIFY gradientStopsChanged FINAL)
    Q_PROPERTY(int resolution READ resolution WRITE setResolution NOTIFY resolutionChanged FINAL)

public:
    explicit LightingGradient(QObject *parent = nullptr);

    QColor colorAt(double value);

    void setResolution(int newResolution);
    int resolution() const;

    QVariantList gradientStops() const;
    void setGradientStops(const QVariantList &newGradientStops);

public slots:
    void updateGradientStops();
    void updateColors();

signals:
    void gradientStopsChanged();
    void resolutionChanged();

protected:
private:
    int _resolution;
    QVector<QColor> _colors;
    QVariantList _gradientStops;
};

#endif // GRADIENT_H
