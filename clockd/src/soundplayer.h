#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <QMediaPlayer>
#include <QObject>

class SoundPlayer : public QObject
{
    Q_OBJECT
public:
    explicit SoundPlayer(QObject *parent = nullptr);

    QString sound() const;
    void setSound(const QString &newSound);

public slots:
    void start();
    void stop();

signals:
    void soundChanged();

private slots:
    void onPositionChanged(qint64 position);

private:
    int _repeats;
    QMediaPlayer _player;

    Q_PROPERTY(QString sound READ sound WRITE setSound NOTIFY soundChanged)
};

#endif // SOUNDPLAYER_H
