#ifndef SOUNDSERVICE_H
#define SOUNDSERVICE_H

#include <QDir>
#include <QObject>

class QMediaPlayer;
class AlarmService;

class SoundService : public QObject
{
    Q_OBJECT
public:
    explicit SoundService(AlarmService *alarmService, QObject *parent = nullptr);
    QStringList availableSounds();

signals:
    void availableSoundsChanged();

private:
    QList<QDir> _soundDiretories;
    Q_PROPERTY(QStringList availableSounds READ availableSounds NOTIFY availableSoundsChanged)
};

#endif // SOUNDSERVICE_H
