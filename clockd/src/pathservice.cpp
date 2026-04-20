#include "pathservice.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QStandardPaths>

#include "version.h"

#include <QLoggingCategory>

namespace {
Q_LOGGING_CATEGORY(self, "paths")
}

QDir PathService::_shareDir = []() {
    if (qEnvironmentVariableIsSet("CLOCKD_SHARE_DIR"))
        return QDir{qEnvironmentVariable("CLOCKD_SHARE_DIR")};

#ifdef QT_DEBUG
    return QDir{QString("%1/clockd/share/clockd").arg(SOURCE_DIR)};
#else
    return QDir{QString("%1/share/clockd").arg(INSTALL_PREFIX)};
#endif
}();

QDir PathService::_confDir = []() {
    QStringList configPaths = QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation);
    if (!configPaths.empty())
        return QDir(configPaths.first());

    QStringList homePaths = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    if (!homePaths.isEmpty()) {
        return QDir(QDir(homePaths.first()).filePath(".config/talking-clock"));
    }

    qCCritical(self) << "failed to init confdir!";
    return QDir{};
}();

PathService::PathService(QObject *parent)
    : QObject(parent)
{
}

QString PathService::shareDir()
{
    return _shareDir.absolutePath();
}

QString PathService::confDir()
{
    return _confDir.absolutePath();
}

QString PathService::configFilePath(const QString &fileName)
{
    return _confDir.filePath(fileName);
}

QString PathService::shareFilePath(const QString &fileName)
{
    return _shareDir.filePath(fileName);
}

bool PathService::writeFile(const QString &fileName, const QByteArray &data)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qCWarning(self) << "could not open file for writing:" << fileName << file.errorString();
        return false;
    }
    
    qint64 written = file.write(data);
    
    if (written != data.size()) {
        qCWarning(self) << "failed to write all data to file:" << fileName << file.errorString();
        return false;
    }
    
    return true;
}

QByteArray PathService::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qCWarning(self) << "could not open file for reading:" << fileName << file.errorString();
        return QByteArray();
    }
    
    return file.readAll();;
}

bool PathService::fileExists(const QString &fileName)
{
    return QFile::exists(fileName);
}
