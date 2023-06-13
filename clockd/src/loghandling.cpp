#include "loghandling.h"

#include <QDebug>
#include <iostream>
#include <syslog.h>

#include "pathservice.h"

QFile LogHandling::LogFile;
QDate LogHandling::LastLogDate;
int LogHandling::KeepLogDuration = 28;

void LogHandling::init()
{
    PathService::create("log", false);
    qInstallMessageHandler(msgHandler);
}

int LogHandling::msgType2syslog(QtMsgType type)
{
    switch (type) {
    case QtMsgType::QtDebugMsg:
        return LOG_DEBUG;
    case QtMsgType::QtInfoMsg:
        return LOG_INFO;
    case QtMsgType::QtWarningMsg:
        return LOG_WARNING;
    case QtMsgType::QtCriticalMsg:
        return LOG_CRIT;
    case QtMsgType::QtFatalMsg:
        return LOG_ALERT;
    default:
        return LOG_ALERT;
    }
}

QChar LogHandling::msgType2char(QtMsgType type)
{
    switch (type) {
    case QtMsgType::QtDebugMsg:
        return 'D';
    case QtMsgType::QtInfoMsg:
        return 'I';
    case QtMsgType::QtWarningMsg:
        return 'W';
    case QtMsgType::QtCriticalMsg:
        return 'C';
    case QtMsgType::QtFatalMsg:
        return 'F';
    default:
        return 'U';
    }
}

QString LogHandling::msgType2string(QtMsgType type)
{
    switch (type) {
    case QtMsgType::QtDebugMsg:
        return "Debug";
    case QtMsgType::QtInfoMsg:
        return "Info";
    case QtMsgType::QtWarningMsg:
        return "Warning";
    case QtMsgType::QtCriticalMsg:
        return "Critical";
    case QtMsgType::QtFatalMsg:
        return "Fatal";
    default:
        return "Unknown";
    }
}

void LogHandling::msgHandler(const QtMsgType type,
                             const QMessageLogContext &context,
                             const QString &msg)
{
    QDateTime now = QDateTime::currentDateTime();
    if (now.date() != LastLogDate) {
        createNewFile(now);
    }

    LogRecord log{now, type, context.category, msg};
    auto line = log.toLine();

    if (type == QtDebugMsg || type == QtInfoMsg)
        std::cout << line.toStdString() << std::flush;
    else {
        std::cerr << line.toStdString() << std::flush;
        LogFile.flush();
    }

    LogFile.write(line.toUtf8());
    LastLogDate = now.date();
}

void LogHandling::createNewFile(const QDateTime &now)
{
    auto date = now.date().toString(Qt::ISODate);
    auto filePath = PathService::create(QString("%1.log").arg(date));
    LogFile.setFileName(filePath);
    if (!LogFile.open(QIODevice::Append)) {
        std::cerr << "cannot open log file:" << filePath.toStdString()
                  << LogFile.errorString().toStdString() << std::endl;
    }
}

LogHandling::LogHandling(){}

QString LogRecord::toLine()
{
    QString line = "[%1] %2: (%3) %4\n";
    line = line.arg(LogHandling::msgType2char(type),
                    time.toString(Qt::ISODateWithMs),
                    category,
                    msg);
    return line;
}
