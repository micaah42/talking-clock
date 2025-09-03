#ifndef SOUNDSERVICE_H
#define SOUNDSERVICE_H

#include <QDir>

#include <QObject>
#include <QQmlEngine>
#include <QSoundEffect>
#include <QTimer>

#include "setting.h"

class SoundService : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QStringList availableSounds READ availableSounds NOTIFY availableSoundsChanged)
    Q_PROPERTY(double volume READ volume WRITE setVolume NOTIFY volumeChanged FINAL)

public:
    explicit SoundService(QObject *parent = nullptr);
    QStringList availableSounds();

    double volume() const;
    void setVolume(double newVolume);

public slots:
    QString displayName(const QString &soundPath);
    void playClickEffect();
    void refresh();

signals:
    void availableSoundsChanged();
    void volumeChanged();

private:
    QDir _soundsFolder;
    QStringList _sounds;
    Setting<double> _volume;
    QSoundEffect _clickEffect;
};

#endif // SOUNDSERVICE_H
