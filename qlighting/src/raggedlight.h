#ifndef RAGGEDLIGHT_H
#define RAGGEDLIGHT_H

#include <QObject>

#include "lightmode.h"

class RaggedLight : public AnimatedLightMode
{
    Q_OBJECT
    Q_PROPERTY(double multiplier READ multiplier WRITE setMultiplier NOTIFY multiplierChanged FINAL)
    Q_PROPERTY(double chunkSize READ chunkSize WRITE setChunkSize NOTIFY chunkSizeChanged FINAL)
    Q_PROPERTY(double saturation READ saturation WRITE setSaturation NOTIFY saturationChanged FINAL)
    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged FINAL)
public:
    explicit RaggedLight(Lighting &lighting);

    virtual QString name() const override;

    double multiplier() const;
    void setMultiplier(double newMultiplier);

    double chunkSize() const;
    void setChunkSize(double newChunkSize);

    double saturation() const;
    void setSaturation(double newSaturation);

    double value() const;
    void setValue(double newValue);

signals:
    void multiplierChanged();
    void chunkSizeChanged();
    void saturationChanged();
    void valueChanged();

protected:
    virtual void animate(double delta) override;

private:
    double _multiplier;
    double _chunkSize;
    double _saturation;
    double _value;
};

#endif // RAGGEDLIGHT_H
