#ifndef WAVINGLIGHT_H
#define WAVINGLIGHT_H

#include <QColor>
#include <QObject>

#include <setting.h>

#include "lightmode.h"
#include "qlighting_global.h"

class WavingLight : public AnimatedLightMode
{
    Q_OBJECT
    QLIGHTING_SINGLETON

    Q_PROPERTY(int length READ length WRITE setLength NOTIFY lengthChanged FINAL)
    Q_PROPERTY(QColor a READ a WRITE setA NOTIFY aChanged FINAL)
    Q_PROPERTY(QColor b READ b WRITE setB NOTIFY bChanged FINAL)

public:
    explicit WavingLight();

    virtual QString name() const override;
    virtual Type type() const override;

    int length() const;
    void setLength(int newLength);

    QColor a() const;
    void setA(const QColor &newA);

    QColor b() const;
    void setB(const QColor &newB);

signals:
    void lengthChanged();
    void aChanged();
    void bChanged();

protected:
    virtual void render(double delta, QList<Pixel *> &pixels) override;

private:
    Setting<int> _length;
    Setting<QColor> _a;
    Setting<QColor> _b;
    double _t;
};

#endif // WAVINGLIGHT_H
