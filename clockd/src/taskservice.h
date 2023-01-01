#ifndef TASKSERVICE_H
#define TASKSERVICE_H

#include <QJsonArray>
#include <QMap>
#include <QObject>
#include <QProcess>

class TaskService : public QObject
{
    Q_OBJECT
public:
    explicit TaskService(QObject *parent = nullptr);
    void startProcess(const QString &program, const QStringList &args);
    QJsonArray processes() const;

signals:
    void processesChanged();

private slots:
    void onReadyRead();
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    // utilities
    static QString describe(const QProcess *process);

    QMap<QProcess *, QString> _outputs;
    Q_PROPERTY(QJsonArray processes READ processes NOTIFY processesChanged)
};

#endif // TASKSERVICE_H
