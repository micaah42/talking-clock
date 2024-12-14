#ifndef CPUGRAPH_H
#define CPUGRAPH_H

#include <QImage>
#include <QQuickPaintedItem>

#include "cpumonitor.h"

class LiveGraph : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(double duration READ duration WRITE setDuration NOTIFY durationChanged FINAL)
    Q_PROPERTY(QList<QColor> colors READ colors WRITE setColors NOTIFY colorsChanged FINAL)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)

public:
    LiveGraph();

    double duration() const;
    void setDuration(double newDuration);

    QList<QColor> colors() const;
    void setColors(const QList<QColor> &newColors);

    QColor color() const;
    void setColor(const QColor &newColor);

public slots:
    void newValues(const QList<double> &values, qint64 t);
    void reset();

signals:
    void durationChanged();
    void colorsChanged();
    void colorChanged();

protected:
    virtual void paint(QPainter *painter) override;

private:
    QList<double> _values;
    double _t;

    QList<double> _prevValues;
    double _prevT;

    QImage _image;
    double m_duration;
    QColor m_color;
    QList<QColor> m_colors;
};

class CPUGraph : public LiveGraph
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(CPUMonitor *cpu READ cpu WRITE setCpu NOTIFY cpuChanged FINAL)

public:
    CPUGraph();
    ~CPUGraph();

    CPUMonitor *cpu() const;
    void setCpu(CPUMonitor *newCpu);

signals:
    void cpuChanged();

protected:
    void onUsagesChanged();

private:
    CPUMonitor *m_cpu = nullptr;
};

#endif // CPUGRAPH_H
