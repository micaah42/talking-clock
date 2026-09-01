#include "chatbotresponse.h"

#include <QDir>
#include <QFile>
#include <QLoggingCategory>
#include <QNetworkReply>
#include <QProcess>
#include <QStandardPaths>
#include <QUuid>

namespace {
Q_LOGGING_CATEGORY(self, "chatbotresponse", QtWarningMsg)
}

ChatBotResponse::ChatBotResponse(QNetworkReply *reply, QObject *parent)
    : QObject{parent}
    , _reply{reply}
    , _done{false}
    , _ttsStatus{TtsStatus::NotStarted}
{}

bool ChatBotResponse::done() const
{
    return _done;
}

void ChatBotResponse::setDone(bool newDone)
{
    if (_done == newDone)
        return;

    qCInfo(self) << this << "done:" << newDone;

    _done = newDone;
    emit doneChanged();

    if (_done && !_text.isEmpty()) {
        startTtsProcess();
    }
}

QString ChatBotResponse::text() const
{
    return _text;
}

void ChatBotResponse::setText(const QString &newText)
{
    if (_text == newText)
        return;

    _text = newText;
    emit textChanged();
}

ChatBotResponse::~ChatBotResponse()
{
    cleanupReply();
    cleanupTtsProcess();
    cleanupTtsFile();
}

ChatBotResponse::TtsStatus ChatBotResponse::ttsStatus() const
{
    return _ttsStatus;
}

void ChatBotResponse::setTtsStatus(TtsStatus newStatus)
{
    if (_ttsStatus == newStatus)
        return;

    qCInfo(self) << this << "ttsStatus:" << newStatus;

    _ttsStatus = newStatus;
    emit ttsStatusChanged();
}

QString ChatBotResponse::ttsOutputFile() const
{
    return _ttsOutputFile;
}

void ChatBotResponse::setTtsOutputFile(const QString &newFilePath)
{
    if (_ttsOutputFile == newFilePath)
        return;

    qCInfo(self) << this << "ttsOutputFile:" << newFilePath;

    _ttsOutputFile = newFilePath;
    emit ttsOutputFileChanged();
}

void ChatBotResponse::cleanupReply()
{
    if (_reply) {
        _reply->disconnect();
        _reply->deleteLater();
        _reply = nullptr;
    }
}

void ChatBotResponse::cleanupTtsFile()
{
    if (!_ttsOutputFile.isEmpty()) {
        QFile file(_ttsOutputFile);
        if (file.exists()) {
            if (file.remove()) {
                qCInfo(self) << this << "Removed TTS file:" << _ttsOutputFile;
            } else {
                qCWarning(self) << this << "Failed to remove TTS file:" << _ttsOutputFile;
            }
        }
        _ttsOutputFile.clear();
    }
}

void ChatBotResponse::cleanupTtsProcess()
{
    if (_ttsProcess) {
        _ttsProcess->disconnect();
        if (_ttsProcess->state() == QProcess::Running) {
            _ttsProcess->terminate();
            if (!_ttsProcess->waitForFinished(3000)) {
                _ttsProcess->kill();
            }
        }
        _ttsProcess->deleteLater();
        _ttsProcess = nullptr;
    }
}

QByteArray getEnvOr(const char *variable, const char *defaultValue)
{
    if (qEnvironmentVariableIsSet(variable))
        return qgetenv(variable);

    return defaultValue;
}

void ChatBotResponse::startTtsProcess()
{
    // static const QString LLamaTttsProgram = getEnvOr("LLAMA_TTS_PROGRAM", "/usr/bin/llama-tts");
    static const QString EspeakTttsProgram = getEnvOr("ESPEAK_TTS_PROGRAM", "/usr/bin/espeak");
    qCInfo(self) << this << "Starting TTS process for text:" << _text.left(50);

    this->setTtsStatus(TtsStatus::Processing);

    // Create output file path in temp directory
    QDir tempDir{QStandardPaths::writableLocation(QStandardPaths::TempLocation)};
    auto idString = QUuid::createUuid().toString(QUuid::WithoutBraces);
    this->setTtsOutputFile(tempDir.filePath(QString("tts_%1.wav").arg(idString)));

    // Create TTS process
    this->cleanupTtsProcess();
    _ttsProcess = new QProcess(this);

    auto finished = QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished);
    connect(_ttsProcess, finished, this, &ChatBotResponse::onTtsProcessFinished);
    connect(_ttsProcess, &QProcess::errorOccurred, this, &ChatBotResponse::onTtsProcessError);

    // Build arguments for llama-tts

    _ttsProcess->start(
        EspeakTttsProgram,
        {
            "-a",
            "150", // amplitude

            "-s",
            "160", // speed

            "-w",
            _ttsOutputFile, // output wave file

            _text,
        }
    );
    qCInfo(self) << this << _ttsProcess->program() << _ttsProcess->arguments();
}

void ChatBotResponse::onTtsProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qCInfo(self) << this << "TTS process finished with exit code:" << exitCode << "status:" << exitStatus;

    if (exitStatus != QProcess::NormalExit || exitCode != 0) {
        qCWarning(self) << this << "TTS process error output:" << _ttsProcess->readAllStandardError();
        emit errorOccurred("TTS process failed with exit code: " + QString::number(exitCode));
        this->setTtsStatus(TtsStatus::Error);
        this->cleanupTtsProcess();
        return;
    }

    if (!QFile::exists(_ttsOutputFile)) {
        qCWarning(self) << this << "TTS file does not exist!" << _ttsProcess->readAllStandardError();
        emit errorOccurred("TTS file does not exist!");
        this->setTtsStatus(TtsStatus::Error);
        this->cleanupTtsProcess();
        return;
    }

    this->setTtsStatus(TtsStatus::Done);
    this->cleanupTtsProcess();
}

void ChatBotResponse::onTtsProcessError(QProcess::ProcessError error)
{
    qCWarning(self) << this << "TTS process error:" << error;
    emit errorOccurred("TTS process error: " + QString::number(error));
    this->setTtsStatus(TtsStatus::Error);
    this->cleanupTtsProcess();
}
QString ChatBotResponse::prompt() const
{
    return _prompt;
}

void ChatBotResponse::setPrompt(const QString &newPrompt)
{
    if (_prompt == newPrompt)
        return;
    _prompt = newPrompt;
    emit promptChanged();
}
