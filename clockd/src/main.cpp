#include <QCursor>
#include <QDebug>
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include <qobjectregistry.h>

#include "about.h"
#include "actionday.h"
#include "alarmservice.h"
#include "clientmanager.h"
#include "cpugraph.h"
#include "cpumonitor.h"
#include "eventfilter.h"
#include "fontservice.h"
#include "lighting.h"
#include "lightingdisplay.h"
#include "loghandling.h"
#include "palette.h"
#include "pathservice.h"
#include "pixel.h"
#include "prettyrandomlight.h"
#include "pulsatinglight.h"
#include "soundservice.h"
#include "spacetheme.h"
#include "staticlight.h"
#include "system.h"
#include "wavinglight.h"
#include "websocketserver.h"

void printApplicationStart();

int main(int argc, char *argv[])
{
    if (true || qEnvironmentVariableIsEmpty("QT_IM_MODULE")) {
        qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    }

    QCoreApplication::setApplicationName("talking-clock");
    QCoreApplication::setOrganizationName("micaah");
    QCoreApplication::setOrganizationDomain("org");
    QCoreApplication::setApplicationVersion("0.1");
    LogHandling::init();
    PathService::init();

    printApplicationStart();

    QGuiApplication::setQuitOnLastWindowClosed(false);
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon{":/favicon.ico"});

#ifndef Q_PROCESSOR_X86_64
    app.setOverrideCursor(QCursor(Qt::BlankCursor));
#endif

    QQmlApplicationEngine engine;
    engine.addImportPath(":/");

    QQuickStyle::setStyle("Material");

    QObjectRegistry remoting;
    WebSocketServer server{remoting};
    remoting.registerObject("remoting", &remoting);

    Palette palette;
    qmlRegisterSingletonInstance<Palette>("Clock", 1, 0, "ColorService", &palette);

    FontService fontService{&engine};
    qmlRegisterSingletonInstance("Clock", 1, 0, "FontService", &fontService);

    AlarmService alarms(500);
    qmlRegisterType<Alarm>("Clock", 1, 0, "Alarm");
    qmlRegisterSingletonInstance("Clock", 1, 0, "AlarmService", &alarms);
    remoting.registerObject("alarms", &alarms);

    SoundService soundService(&alarms);
    qmlRegisterSingletonInstance("Clock", 1, 0, "SoundService", &soundService);
    remoting.registerObject("sounds", &soundService);

    EventFilter eventFilter;
    app.installEventFilter(&eventFilter);
    qmlRegisterSingletonInstance<EventFilter>("Clock", 1, 0, "EventFilter", &eventFilter);

    ActionDayService actionDayManager;
    qmlRegisterSingletonInstance("Clock", 1, 0, "ActionDayManager", &actionDayManager);
    remoting.registerObject("actiondays", &actionDayManager);

    About about;
    qmlRegisterSingletonInstance("Clock", 1, 0, "About", &about);

    SpaceTheme spaceTheme;
    qmlRegisterUncreatableType<SpaceTheme>("Clock", 1, 0, "SpaceTheme", "");
    qmlRegisterSingletonInstance("Clock", 1, 0, "SpaceTheme", &spaceTheme);

    CPUMonitor cpu;
    qmlRegisterType<CPUGraph>("Clock", 1, 0, "CPUGraph");
    qmlRegisterUncreatableType<CPUMonitor>("Clock", 1, 0, "CPUMonitor", "");
    qmlRegisterSingletonInstance("Clock", 1, 0, "CPUMonitor", &cpu);

    Lighting lighting{300};
    qmlRegisterUncreatableType<Pixel>("Clock", 1, 0, "Pixel", "");
    qmlRegisterUncreatableType<Lighting>("Clock", 1, 0, "Lighting", "");
    qmlRegisterUncreatableType<LightMode>("Clock", 1, 0, "LightMode", "");
    qmlRegisterUncreatableType<StaticLight>("Clock", 1, 0, "StaticLight", "");
    qmlRegisterUncreatableType<WavingLight>("Clock", 1, 0, "WavingLight", "");
    qmlRegisterUncreatableType<PrettyRandomLight>("Clock", 1, 0, "PrettyRandomLight", "");
    qmlRegisterUncreatableType<PulsatingLight>("Clock", 1, 0, "PulsatingLight", "");
    qmlRegisterUncreatableType<MonoRotationLight>("Clock", 1, 0, "MonoRotationLight", "");
    qmlRegisterType<LightingDisplay>("Clock", 1, 0, "LightingDisplay");
    qmlRegisterSingletonInstance("Clock", 1, 0, "Lighting", &lighting);
    remoting.registerObject("lights", &lighting);

    System system;
    qmlRegisterUncreatableType<System>("Clock", 1, 0, "System", "");
    qmlRegisterSingletonInstance("Clock", 1, 0, "System", &system);

    ClientService clientService{server};
    qmlRegisterUncreatableType<Client>("Clock", 1, 0, "Client", "");
    qmlRegisterUncreatableType<ClientService>("Clock", 1, 0, "ClientService", "");
    qmlRegisterSingletonInstance("Clock", 1, 0, "ClientService", &clientService);

    const QUrl url("qrc:/Clock/Main.qml");
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

    engine.addImportPath(":/");
    engine.addPluginPath(":/");

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
