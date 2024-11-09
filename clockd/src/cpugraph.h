#ifndef CPUGRAPH_H
#define CPUGRAPH_H

#include <QQuickPaintedItem>

#include "cpumonitor.h"

class CPUGraph : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int duration READ duration WRITE setDuration NOTIFY durationChanged FINAL)
    Q_PROPERTY(CPUMonitor *cpu READ cpu WRITE setCpu NOTIFY cpuChanged FINAL)
public:
    CPUGraph();
    ~CPUGraph();

    CPUMonitor *cpu() const;
    void setCpu(CPUMonitor *newCpu);

    int duration() const;
    void setDuration(int newDuration);

signals:
    void cpuChanged();
    void durationChanged();

protected:
    virtual void paint(QPainter *painter) override;
    void onUsagesChanged();

private:
    CPUMonitor *m_cpu = nullptr;
    QList<QList<double>> _usages;
    QList<qint64> _ts;
    int m_duration;
};

#endif // CPUGRAPH_H
