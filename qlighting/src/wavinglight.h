#ifndef WAVINGLIGHT_H
#define WAVINGLIGHT_H

#include <QColor>
#include <QObject>
#include <QTimer>

#include "lightmode.h"

class WavingLight : public LightMode
{
    Q_OBJECT
    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged FINAL)
    Q_PROPERTY(double speed READ speed WRITE setSpeed NOTIFY speedChanged FINAL)
    Q_PROPERTY(int length READ length WRITE setLength NOTIFY lengthChanged FINAL)
    Q_PROPERTY(QColor a READ a WRITE setA NOTIFY aChanged FINAL)
    Q_PROPERTY(QColor b READ b WRITE setB NOTIFY bChanged FINAL)

public:
    explicit WavingLight(Lighting &lighting);

    double speed() const;
    void setSpeed(double newSpeed);

    int length() const;
    void setLength(int newLength);

    QColor a() const;
    void setA(const QColor &newA);

    QColor b() const;
    void setB(const QColor &newB);

    int interval() const;
    void setInterval(int newInterval);

signals:
    void speedChanged();
    void lengthChanged();
    void aChanged();
    void bChanged();
    void intervalChanged();

protected:
    void setupGradient();
    void onTimeout();

private:
    QList<QColor> _colors;
    QTimer _timer;

    double m_speed;
    int m_length;
    QColor m_a;
    QColor m_b;
    int m_shift;

    // LightMode interface
public:
    virtual QString name() const override { return "Waving"; };
    virtual void start() override { _timer.start(); };
    virtual void stop() override { _timer.stop(); };
};

#endif // WAVINGLIGHT_H
