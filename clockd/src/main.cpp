#include <QCursor>
#include <QDebug>
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "alarmservice.h"
#include "dataservice.h"
#include "eventfilter.h"
#include "fontservice.h"
#include "loghandling.h"
#include "palette.h"
#include "pathservice.h"
#include "remoting.h"
#include "settingsservice.h"
#include "soundservice.h"
#include "taskservice.h"

void printApplicationStart();

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    if (true || qEnvironmentVariableIsEmpty("QT_IM_MODULE")) {
        qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    }

    QCoreApplication::setOrganizationName("micaah");
    QCoreApplication::setOrganizationDomain("org");
    QCoreApplication::setApplicationName("talking-clock");
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

    Remoting remoting;
    remoting.registerObject("remoting", &remoting);

    SettingsService settingsService;
    qmlRegisterSingletonInstance<SettingsService>("Clock", 1, 0, "SettingsService", &settingsService);
    remoting.registerObject("settings", &settingsService);

    Palette palette{&settingsService};
    qmlRegisterSingletonInstance<Palette>("Clock", 1, 0, "ColorService", &palette);

    FontService fontService{&settingsService, &engine};
    qmlRegisterSingletonInstance("Clock", 1, 0, "FontService", &fontService);

    AlarmService alarms(500);
    qmlRegisterSingletonInstance("Clock", 1, 0, "AlarmService", &alarms);
    // qmlRegisterType<Alarm>("Clock", 1, 0, "Alarm");
    remoting.registerObject("alarms", &alarms);

    SoundService soundService(&alarms);
    qmlRegisterSingletonInstance("Clock", 1, 0, "SoundService", &soundService);
    remoting.registerObject("sounds", &soundService);

    TaskService taskService;
    qmlRegisterSingletonInstance("Clock", 1, 0, "TaskService", &taskService);

    EventFilter eventFilter;
    app.installEventFilter(&eventFilter);
    qmlRegisterSingletonInstance<EventFilter>("Clock", 1, 0, "EventFilter", &eventFilter);

    DataService dataService;
    qmlRegisterSingletonInstance<DataService>("Clock", 1, 0, "DataService", &dataService);
    remoting.registerObject("data", &dataService);

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
