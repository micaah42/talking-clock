#include "soundplayer.h"

#include <QDir>
#include <QLoggingCategory>
#include <QMediaPlaylist>

namespace {
Q_LOGGING_CATEGORY(self, "player");
}

SoundPlayer::SoundPlayer(QObject *parent)
    : QObject{parent}
{
    connect(&_player, &QMediaPlayer::positionChanged, this, &SoundPlayer::onPositionChanged);
}

QString SoundPlayer::sound() const
{
    return _player.media().request().url().toString();
}

void SoundPlayer::setSound(const QString &sound)
{
    QDir sounds(":/sounds");
    auto matches = sounds.entryList({QString("%1.*").arg(sound)}, QDir::Files);
    if (matches.empty()) {
        qCCritical(self) << "failed to locate sound:" << sound;
        return;
    }

    QUrl url = QString("qrc%1").arg(sounds.filePath(matches.first()));
    auto playlist = new QMediaPlaylist{};
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    playlist->addMedia(url);
    _player.setPlaylist(playlist);
}

void SoundPlayer::start()
{
    _player.setVolume(10);
    _player.play();
    _repeats = 0;
}

void SoundPlayer::stop()
{
    _player.stop();
}

void SoundPlayer::onPositionChanged(qint64 position)
{
    if (position < _player.duration() || !_player.isAudioAvailable()) {
        return;
    }

    // increment volume if possible
    if (_player.volume() < 100) {
        _player.setVolume(_player.volume() + 10);
        qCInfo(self) << "playing louder..." << _player.volume();
    }
}
