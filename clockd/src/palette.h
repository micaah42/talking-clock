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

signals:
    void colorsChanged();
    void frontlightChanged();
    void backlightChanged();

private:
    void updateColors();
    SettingsService *_settings;
    int _frontlight, _backlight;
    QVariantMap _colors;
    QMap<QString, QColor> _baseColors;

    // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
    Q_PROPERTY(QVariantMap colors READ colors NOTIFY colorsChanged)
};

#endif // PALETTE_H
