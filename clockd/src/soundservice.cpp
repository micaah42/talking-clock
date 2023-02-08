#include "soundservice.h"

#include <QLoggingCategory>
#include <QMediaPlaylist>

namespace {
Q_LOGGING_CATEGORY(self, "sounds")
}

SoundService::SoundService(QObject *parent)
    : QObject(parent), _soundsFolder{"/usr/share/clockd/sounds"}
{
    // stop player automatically after specified amount of time
    connect(&_timer, &QTimer::timeout, this, &SoundService::stop);
    _timer.setInterval(std::chrono::seconds(45));

    // make louder
    connect(&_player, &QMediaPlayer::positionChanged, this, &SoundService::onPositionChanged);
    refresh();
}

QStringList SoundService::availableSounds()
{
    return _sounds;
}

void SoundService::refresh()
{
    _soundsFolder.refresh();
    _sounds = _soundsFolder.entryList({"*.mp3", "*.wav"});
    qCInfo(self) << "found" << _sounds.size() << "sounds";
    emit availableSoundsChanged();
}

void SoundService::play(const QString &sound)
{
    auto playlist = new QMediaPlaylist{};
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    playlist->addMedia(QUrl{"file://" + _soundsFolder.filePath(sound)});

    _player.setPlaylist(playlist);
    _player.setVolume(40);
    _player.play();
    _timer.start();
}

void SoundService::stop()
{
    // _player.setPlaylist(nullptr);
    _player.stop();
}

void SoundService::onPositionChanged(int position)
{
    if (position < _player.duration() || !_player.isAudioAvailable()) {
        // we actually want to handle the repeat-restarts only so return when not on end
        return;
    }

    // increment volume if possible
    if (_player.volume() < 100) {
        _player.setVolume(_player.volume() + 15);
        qCInfo(self) << "playing louder..." << _player.volume();
    }
}
