#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaService>

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    auto audioDevices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    for (auto const &device : audioDevices)
        qDebug() << device.deviceName() << device.supportedCodecs();

    qDebug() << "default:" << QAudioDeviceInfo::defaultOutputDevice().deviceName();

    auto player = new QMediaPlayer;
    player->setMedia(QUrl{"qrc:/loudcow.wav"});
    player->setVolume(200);
    player->play();

    qDebug() << "availability:" << player->isAudioAvailable() << player->availability();
    qDebug() << "playing...";
    return app.exec();
}
