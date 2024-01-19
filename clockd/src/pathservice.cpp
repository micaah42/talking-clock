#include "pathservice.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>

QString PathService::AppHome;

QString PathService::create(const QString &filename, bool file)
{
    if (file) {
        return QDir(AppHome).filePath(filename);
    }
    else {
        QDir(AppHome).mkpath(filename);
        return QDir(AppHome).absoluteFilePath(filename);
    }
}

void PathService::init()
{
    QStringList homeLocations = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    if (homeLocations.isEmpty()) {
        qCritical() << "no home locations!";
        homeLocations.append("");
    }
    AppHome = QDir(homeLocations.first()).filePath(QCoreApplication::applicationName());
    if (!QDir().mkpath(AppHome)) {
        qCritical() << "error making homedir!";
    };
    qInfo() << "set paths.";
}
