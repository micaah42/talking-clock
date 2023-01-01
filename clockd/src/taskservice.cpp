#include "taskservice.h"

#include <QJsonObject>
#include <QLoggingCategory>

namespace {
Q_LOGGING_CATEGORY(self, "tasks");
}

TaskService::TaskService(QObject *parent)
    : QObject{parent}
{
    qCInfo(self) << "init taskservice";
}

void TaskService::startProcess(const QString &program, const QStringList &args)
{
    auto process = new QProcess(this);
    _outputs[process] = "";

    auto ExitSignal = QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished);
    connect(process, ExitSignal, this, &TaskService::onFinished);
    connect(process, &QProcess::readyRead, this, &TaskService::onReadyRead);

    process->start(program, args);
    qCInfo(self) << "running:" << program << args;
    emit processesChanged();
}

void TaskService::onReadyRead()
{
    auto process = qobject_cast<QProcess *>(sender());
    Q_ASSERT(process);

    auto output = process->readAllStandardOutput();
    auto error = process->readAllStandardError();

    qCInfo(self) << "stdout" << process->program() << process->arguments() << ":" << output;
    _outputs[process] = output;
    if (!error.isEmpty()) {
        _outputs[process] = error;
        qCInfo(self) << "stderr" << process->program() << process->arguments() << ":" << error;
    }
    emit processesChanged();
}

void TaskService::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    auto process = qobject_cast<QProcess *>(sender());
    Q_ASSERT(process);

    if (exitCode != 0) {
        qCWarning(self) << describe(process) << "failed with code" << exitCode;
    }
    else if (exitStatus != QProcess::NormalExit) {
        qCWarning(self) << describe(process) << "process exited with status" << exitStatus;
    }
    else {
        qCInfo(self) << "finished normally:" << process->program() << process->arguments();
    }

    _outputs.remove(process);
    process->deleteLater();
    emit processesChanged();
}

QString TaskService::describe(const QProcess *process)
{
    return QString("%1 (&2)").arg(process->program(), process->arguments().join(" "));
}

QJsonArray TaskService::processes() const
{
    QJsonArray arr;
    for (auto const &process : _outputs.keys()) {
        arr.append(QJsonObject{{"program", process->program()},
                               {"args", process->arguments().join(' ')},
                               {"output", _outputs[process]}});
    }
    return arr;
}
