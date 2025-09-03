#include <QCursor>
#include <QDebug>
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#ifndef TINYORM_USING_QTSQLDRIVERS
#define TINYORM_USING_QTSQLDRIVERS
#endif

#include <orm/db.hpp>

#include "loghandling.h"
#include "pathservice.h"

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

    QQuickStyle::setStyle("Material");

#ifndef Q_PROCESSOR_X86_64
    app.setOverrideCursor(QCursor(Qt::BlankCursor));
#endif

    auto manager = Orm::DB::create({
        {"driver", "QSQLITE"},
        {"database", qEnvironmentVariable("DB_DATABASE", "./usr/share/clockd/actiondays.sqlite")},
        {"foreign_key_constraints", qEnvironmentVariable("DB_FOREIGN_KEYS", "true")},
        {"check_database_exists", true},
        {"prefix", ""},
    });

    QQmlApplicationEngine engine;
    engine.addImportPath(":/");

    qInfo() << "loading view...";
    engine.loadFromModule("Clock", "MainWindow");
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
