#include <QCursor>
#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

// #define __ARM_PCS_VFin

#include "alarmservice.h"
#include "fontservice.h"
#include "loghandling.h"
#include "palette.h"
#include "pathservice.h"
#include "settingsservice.h"
#include "soundplayer.h"
#include "soundservice.h"
#include "taskservice.h"

#include "remoting.h"

void printApplicationStart();

int main(int argc, char *argv[])
{
    if (true || qEnvironmentVariableIsEmpty("QT_IM_MODULE")) {
        qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    }

    QCoreApplication::setOrganizationDomain("org");
    QCoreApplication::setOrganizationName("micaah");
    QCoreApplication::setApplicationName("talking-clock");
    QCoreApplication::setApplicationVersion("0.1");
    LogHandling::init();
    PathService::init();

    printApplicationStart();

    // qmlRegisterType<SoundPlayer>("Clock", 1, 0, "SoundPlayer");

#ifndef Q_PROCESSOR_X86_64
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication::setQuitOnLastWindowClosed(false);

    QGuiApplication app(argc, argv);
#ifndef Q_PROCESSOR_X86_64
    app.setOverrideCursor(QCursor(Qt::BlankCursor));
#endif

    QQmlApplicationEngine engine;
    QQuickStyle::setStyle("Material");

    // first service to set up is the settings service
    auto settingsService = new SettingsService;
    auto palette = new Palette(settingsService);
    auto fontService = new FontService(settingsService, &engine);
    auto alarms = new AlarmService(500);
    auto soundService = new SoundService(alarms);
    auto taskService = new TaskService;

    // inject service(s)
    qmlRegisterSingletonInstance<SettingsService>("Clock", 1, 0, "SettingsService", settingsService);
    qmlRegisterUncreatableType<Alarm>("Clock", 1, 0, "Alarm", "alarm created");
    engine.rootContext()->setContextProperty("colorService", palette);
    engine.rootContext()->setContextProperty("fontService", fontService);
    engine.rootContext()->setContextProperty("alarms", alarms);
    engine.rootContext()->setContextProperty("soundService", soundService);
    engine.rootContext()->setContextProperty("taskService", taskService);

    Remoting remoting;
    remoting.registerObject("remoting", &remoting);
    remoting.registerObject("settings", settingsService);
    remoting.registerObject("alarms", alarms);
    remoting.registerObject("sounds", soundService);

    const QUrl url("qrc:/Main.qml");
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) {
                qWarning() << "loading went wrong!";
                QCoreApplication::exit(-2);
            }
        },
        Qt::QueuedConnection);

    qInfo() << "loading view...";
    engine.load(url);
    qInfo() << "done!";

    return app.exec();
}

void printApplicationStart()
{
    auto appName = QCoreApplication::applicationName();
    auto buildTime = QString("build time: %1 %2").arg(__DATE__, __TIME__);
    auto appVersion = QString("application version: %1").arg(QCoreApplication::applicationVersion());
    auto qtVersion = QString("qt version: %1").arg(QT_VERSION_STR);
    qInfo().noquote() << QString().fill('#', 82);
    qInfo().noquote() << "#  " + appName.leftJustified(75, ' ') << "  #";
    qInfo().noquote() << "#  " + QString().fill('-', 75) << "  #";
    qInfo().noquote() << "#  " + buildTime.leftJustified(75, ' ') << "  #";
    qInfo().noquote() << "#  " + appVersion.leftJustified(75, ' ') << "  #";
    qInfo().noquote() << "#  " + qtVersion.leftJustified(75, ' ') << "  #";
    qInfo().noquote() << "#  " + QString("by micaah42").rightJustified(75, ' ') << "  #";
    qInfo().noquote() << QString().fill('#', 82);
}
