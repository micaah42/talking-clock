#include "soundservice.h"
#include <QMediaPlayer>

SoundService::SoundService(AlarmService *alarmsService, QObject *parent)
    : QObject(parent), _soundDiretories({QDir{":/sounds"}})
{
}

QStringList SoundService::availableSounds()
{
    QStringList sounds;
    for (auto const &dir : _soundDiretories) {
        if (!dir.exists()) {
            qWarning() << "directory does not exist" << dir.path();
        }
        for (auto const &file : dir.entryList()) {
            if (file.endsWith(".mp3") || file.endsWith(".wav")) {
                sounds.append(file.split("/").last().split(".").first());
            }
        }
    }
    return sounds;
}
