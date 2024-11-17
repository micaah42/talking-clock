#include "cpugraph.h"

#include <QDateTime>
#include <QLoggingCategory>
#include <QPainter>
#include <QPainterPath>

namespace {
Q_LOGGING_CATEGORY(self, "cpugraph")
}

CPUGraph::CPUGraph()
    : m_duration{5000}
{}

CPUGraph::~CPUGraph()
{
    if (m_cpu)
        m_cpu->unsubscribe();
}

CPUMonitor *CPUGraph::cpu() const
{
    return m_cpu;
}

void CPUGraph::setCpu(CPUMonitor *newCpu)
{
    if (m_cpu == newCpu)
        return;

    if (m_cpu) {
        disconnect(m_cpu, &CPUMonitor::usagesChanged, this, &CPUGraph::onUsagesChanged);
        m_cpu->unsubscribe();
    }

    m_cpu = newCpu;
    emit cpuChanged();

    if (m_cpu) {
        connect(m_cpu, &CPUMonitor::usagesChanged, this, &CPUGraph::onUsagesChanged);
        m_cpu->subscribe();
    }
}

void CPUGraph::paint(QPainter *painter)
{
    if (_ts.empty()) {
        qCWarning(self) << "too few samples to paint";
        return;
    }

    auto now = _ts.last();

    painter->setRenderHint(QPainter::Antialiasing);

    double pxPerMs = this->width() / (double) m_duration;
    double pxPerPercent = this->height() / 100.;

    QPen pen{QColor{"white"}, 1, Qt::SolidLine, Qt::RoundCap};
    painter->setPen(pen);

    for (auto const &line : std::as_const(_usages)) {
        QPainterPath path;
        path.moveTo(this->width(), this->height() - pxPerPercent * line.last());

        for (int i = line.size() - 1; i >= 0; --i)
            path.lineTo(this->width() - pxPerMs * (now - _ts[i]), this->height() - pxPerPercent * line[i]);

        painter->drawPath(path);
    }
}

void CPUGraph::onUsagesChanged()
{
    auto now = QDateTime::currentMSecsSinceEpoch();
    _ts.append(now);

    auto newUsages = m_cpu->usages();

    if (newUsages.size() != _usages.size()) {
        qCWarning(self) << "plotting history for" << newUsages.size() << "cores";
        _usages.resize(newUsages.size());
    }

    for (int i = 0; i < newUsages.size(); ++i) {
        _usages[i].append(newUsages[i]);
    }

    while (!_ts.empty() && _ts.first() < now - m_duration) {
        for (int i = 0; i < _usages.size(); ++i)
            _usages[i].removeFirst();

        _ts.removeFirst();
    }

    this->update();
}

int CPUGraph::duration() const
{
    return m_duration;
}

void CPUGraph::setDuration(int newDuration)
{
    if (m_duration == newDuration)
        return;

    m_duration = newDuration;
    emit durationChanged();
}
