#include "soundservice.h"

#include <QDirIterator>
#include <QLoggingCategory>

namespace {
Q_LOGGING_CATEGORY(self, "sounds")
}

SoundService::SoundService(QObject *parent)
    : QObject(parent)
    , _soundsFolder{"./usr/share/clockd/sounds"}
    , _volume{"Sounds/Volume", 1.}
{
    this->refresh();
}

QStringList SoundService::availableSounds()
{
    return _sounds;
}

QString SoundService::displayName(const QString &soundPath)
{
    return QFileInfo{soundPath}.baseName();
}

void SoundService::refresh()
{
    _sounds.clear();

    QDirIterator it{
        _soundsFolder.path(),
        {"*.mp3", "*.wav"},
        QDir::Files,
        QDirIterator::Subdirectories | QDirIterator::FollowSymlinks,
    };

    while (it.hasNext()) {
        it.next();
        _sounds.append(it.fileInfo().absoluteFilePath());
    }

    qCInfo(self) << "found" << _sounds.size() << "sounds" << _soundsFolder.exists();
    emit availableSoundsChanged();
}

double SoundService::volume() const
{
    return _volume;
}

void SoundService::setVolume(double newVolume)
{
    if (qFuzzyCompare(_volume, newVolume))
        return;

    _volume = newVolume;
    emit volumeChanged();
}
