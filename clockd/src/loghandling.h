#ifndef LOGHANDLING_H
#define LOGHANDLING_H

#include <QDateTime>
#include <QDir>
#include <QFile>

struct LogRecord
{
    const QDateTime time;
    const QtMsgType type;
    const QString category;
    const QString msg;

    QString toLine();
};

class LogHandling
{
public:
    static int KeepLogDuration;
    static void init();
    static int msgType2syslog(QtMsgType type);
    static QChar msgType2char(QtMsgType type);
    static QString msgType2string(QtMsgType type);

private:
#ifdef MANUAL_LOGFILES
    static QFile LogFile;
    static QDate LastLogDate;
    static void createNewFile(const QDateTime &now);
#endif

    static void msgHandler(const QtMsgType type,
                           const QMessageLogContext &context,
                           const QString &msg);
    LogHandling();
};

#endif // LOGHANDLING_H
