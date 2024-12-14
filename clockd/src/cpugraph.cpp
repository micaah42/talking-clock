#include "cpugraph.h"

#include <QDateTime>
#include <QLoggingCategory>
#include <QPainter>
#include <QPainterPath>
#include <QQuickItemGrabResult>

namespace {
Q_LOGGING_CATEGORY(self, "cpugraph")
}

LiveGraph::LiveGraph()
    : m_duration{5000}
    , m_color{Qt::white}
{
    connect(this, &QQuickItem::heightChanged, this, &LiveGraph::reset);
    connect(this, &QQuickItem::widthChanged, this, &LiveGraph::reset);
    this->reset();
}

CPUGraph::CPUGraph() {}

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

void CPUGraph::onUsagesChanged()
{
    this->newValues(m_cpu->usages().mid(1), QDateTime::currentMSecsSinceEpoch());
}

void LiveGraph::paint(QPainter *painter)
{
    if (_prevValues.size() != _values.size()) {
        qCWarning(self) << "value size mismatch!";
        return;
    }

    painter->setRenderHint(QPainter::Antialiasing);

    double pxPerMs = this->width() / (double) m_duration;
    double pxPerPercent = this->height() / 100.;
    double changedPx = pxPerMs * (_t - _prevT);

    QImage newImage(width(), height(), QImage::Format_ARGB32_Premultiplied);
    newImage.fill(Qt::transparent);

    QPainter pp{&newImage};
    pp.setRenderHint(QPainter::Antialiasing);

    QRectF target{0, 0, width() - changedPx, height()};
    QRectF source{changedPx, 0, width() - changedPx, height()};
    pp.drawImage(target, _image, source);

    for (int i = 0; i < _values.size(); ++i) {
        QPointF from{this->width() - changedPx, this->height() - pxPerPercent * _prevValues[i]};
        QPointF to{this->width(), this->height() - pxPerPercent * _values[i]};

        QPen pen{i < m_colors.size() ? m_colors[i] : m_color, 1, Qt::SolidLine, Qt::RoundCap};
        pp.setPen(pen);
        pp.drawLine(from, to);
    }

    painter->drawImage(_image.rect(), newImage, _image.rect());
    _image = newImage;
}

void LiveGraph::newValues(const QList<double> &values, qint64 t)
{
    _prevValues = _values;
    _values = values;

    _prevT = _t;
    _t = t;

    this->update();
}

void LiveGraph::reset()
{
    _image = QImage{QSize(this->width(), this->height()), QImage::Format_ARGB32_Premultiplied};
    _image.fill(Qt::transparent);
}

double LiveGraph::duration() const
{
    return m_duration;
}

void LiveGraph::setDuration(double newDuration)
{
    if (m_duration == newDuration)
        return;

    m_duration = newDuration;
    emit durationChanged();
}

QList<QColor> LiveGraph::colors() const
{
    return m_colors;
}

void LiveGraph::setColors(const QList<QColor> &newColors)
{
    if (m_colors == newColors)
        return;
    m_colors = newColors;
    emit colorsChanged();
}

QColor LiveGraph::color() const
{
    return m_color;
}

void LiveGraph::setColor(const QColor &newColor)
{
    if (m_color == newColor)
        return;
    m_color = newColor;
    emit colorChanged();
}
