#ifndef SOUNDSERVICE_H
#define SOUNDSERVICE_H

#include <QDir>
#include <QMediaPlayer>
#include <QObject>
#include <QTimer>

class SoundService : public QObject
{
    Q_OBJECT
public:
    explicit SoundService(QObject *parent = nullptr);
    QStringList availableSounds();

public slots:
    void refresh();

    void play(const QString &sound);
    void stop();

signals:
    void availableSoundsChanged();

private slots:
    void onPositionChanged(int position);

private:
    QDir _soundsFolder;
    QStringList _sounds;
    //QMediaPlayer _player;
    QTimer _timer;

    // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
    Q_PROPERTY(QStringList availableSounds READ availableSounds NOTIFY availableSoundsChanged)
};

#endif // SOUNDSERVICE_H
