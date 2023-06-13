#ifndef PALETTE_H
#define PALETTE_H

#include <QColor>
#include <QObject>
#include <QVariantMap>

#include "settingsservice.h"

class Palette : public QObject
{
    Q_OBJECT
public:
    Palette(SettingsService *settings);

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
    SettingsService *_settings;

    QColor _primary, _darkPrimary, _accent, _darkAccent, _background;
    int _frontlight, _backlight;

    // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
    Q_PROPERTY(QColor primary READ primary WRITE setPrimary NOTIFY primaryChanged)
    Q_PROPERTY(QColor darkPrimary READ darkPrimary WRITE setDarkPrimary NOTIFY darkPrimaryChanged)
    Q_PROPERTY(QColor accent READ accent WRITE setAccent NOTIFY accentChanged)
    Q_PROPERTY(QColor background READ background WRITE setBackground NOTIFY backgroundChanged)

    Q_PROPERTY(int frontlight READ frontlight WRITE setFrontlight NOTIFY frontlightChanged)
    Q_PROPERTY(int backlight READ backlight WRITE setBacklight NOTIFY backlightChanged)
};

#endif // PALETTE_H
