#ifndef PULSATINGLIGHT_H
#define PULSATINGLIGHT_H

#include <QColor>
#include <QObject>

#include "lightmode.h"

class Lighting;
class Pixel;

class PulsatingLight : public AnimatedLightMode
{
    Q_OBJECT
    Q_PROPERTY(QColor a READ a WRITE setA NOTIFY aChanged FINAL)
    Q_PROPERTY(QColor b READ b WRITE setB NOTIFY bChanged FINAL)
    Q_PROPERTY(double f1 READ f1 WRITE setF1 NOTIFY f1Changed FINAL)
    Q_PROPERTY(double f2 READ f2 WRITE setF2 NOTIFY f2Changed FINAL)
    Q_PROPERTY(double f3 READ f3 WRITE setF3 NOTIFY f3Changed FINAL)
    Q_PROPERTY(int length READ length WRITE setLength NOTIFY lengthChanged FINAL)
public:
    explicit PulsatingLight(Lighting &parent);
    virtual void animate(double delta) override;

    virtual QString name() const override { return "Pulsating"; };
    double f1() const;
    void setF1(double newF1);

    double f2() const;
    void setF2(double newF2);

    int length() const;
    void setLength(int newLength);

    QColor a() const;
    void setA(const QColor &newA);

    QColor b() const;
    void setB(const QColor &newB);

    double f3() const;
    void setF3(double newF3);

signals:
    void lengthChanged();
    void f1Changed();
    void f2Changed();
    void aChanged();
    void bChanged();

    void f3Changed();

protected:
    void initGradient();

private:
    QList<QColor> _gradient;
    double _t;
    int m_length;

    double m_f1;
    double m_f2;
    QColor m_a;
    QColor m_b;
    double m_f3;
};

#endif // PULSATINGLIGHT_H
