#ifndef PALETTE_H
#define PALETTE_H

#include <QColor>
#include <QObject>
#include <QVariantMap>

#include "setting.h"

class Palette : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor primary READ primary WRITE setPrimary NOTIFY primaryChanged)
    Q_PROPERTY(QColor darkPrimary READ darkPrimary WRITE setDarkPrimary NOTIFY darkPrimaryChanged)
    Q_PROPERTY(QColor accent READ accent WRITE setAccent NOTIFY accentChanged)
    Q_PROPERTY(QColor background READ background WRITE setBackground NOTIFY backgroundChanged)

    Q_PROPERTY(int frontlight READ frontlight WRITE setFrontlight NOTIFY frontlightChanged)
    Q_PROPERTY(int backlight READ backlight WRITE setBacklight NOTIFY backlightChanged)

public:
    Palette(QObject *parent = nullptr);

    const QVariantMap &colors() const;

    double frontlight() const;
    void setFrontlight(double newFrontLight);

    double backlight() const;
    void setBacklight(double newBacklight);

    QColor primary() const;
    void setPrimary(const QColor &newPrimary);

    QColor darkPrimary() const;
    void setDarkPrimary(const QColor &newDarkPrimary);

    QColor accent() const;
    void setAccent(const QColor &newAccent);

    QColor background() const;
    void setBackground(const QColor &newBackground);

signals:
    void colorsChanged();
    void frontlightChanged();
    void backlightChanged();

    void primaryChanged();
    void darkPrimaryChanged();
    void accentChanged();
    void backgroundChanged();

private:
    QColor _primary, _darkPrimary, _accent, _darkAccent, _background;
    QSettings _settings;
    Setting<double> _frontlight;
    Setting<double> _backlight;

    // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
};

#endif // PALETTE_H
