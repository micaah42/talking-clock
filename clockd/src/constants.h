#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>

namespace Path {
static const QString AppHome
    = QDir(QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first())
          .filePath(QCoreApplication::applicationName());
static void init(){};
} // namespace Path
#endif // CONSTANTS_H
