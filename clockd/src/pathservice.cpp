#include "pathservice.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>

QString PathService::AppHome;

QString PathService::homeFile(const QString &filename, bool file)
{
    if (file) {
        return QDir(AppHome).filePath(filename);
    }
    else {
        auto path = QDir(AppHome).filePath(filename);
        QDir().mkpath(path);
        return path;
    }
}

void PathService::init()
{
    QStringList homeLocations = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    if (homeLocations.isEmpty()) {
        qCritical() << "no home locations!";
        homeLocations.append("");
    }
    AppHome = QDir(homeLocations.first()).filePath(QCoreApplication::applicationName() + "-data");
    if (!QDir().mkpath(AppHome)) {
        qCritical() << "error making homedir!";
    };
    qInfo() << "set paths.";
}
