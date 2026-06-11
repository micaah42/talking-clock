#ifndef CHATBOTRESPONSE_H
#define CHATBOTRESPONSE_H

#include <QNetworkReply>
#include <QObject>
#include <QPointer>
#include <QProcess>
#include <QQmlEngine>


class ChatBotResponse : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(QString prompt READ prompt NOTIFY promptChanged FINAL)
    Q_PROPERTY(QString text READ text NOTIFY textChanged FINAL)
    Q_PROPERTY(bool done READ done NOTIFY doneChanged FINAL)
    Q_PROPERTY(TtsStatus ttsStatus READ ttsStatus NOTIFY ttsStatusChanged FINAL)
    Q_PROPERTY(QString ttsOutputFile READ ttsOutputFile NOTIFY ttsOutputFileChanged FINAL)

public:
    enum TtsStatus { NotStarted, Processing, Done, Error };
    Q_ENUM(TtsStatus);

    QString prompt() const;
    QString text() const;
    bool done() const;
    TtsStatus ttsStatus() const;
    QString ttsOutputFile() const;
    ~ChatBotResponse();

signals:
    void errorOccurred(const QString &errorString);
    void textReceived(const QString &text);
    void textChanged();
    void doneChanged();
    void ttsStatusChanged();
    void ttsOutputFileChanged();

    void promptChanged();

protected:
    explicit ChatBotResponse(QNetworkReply *reply, QObject *parent = nullptr);

    friend class LLamaCppService;
    friend class OllamaService;

    void setPrompt(const QString &newPrompt);
    void setText(const QString &newText);
    void setDone(bool newDone);
    void setTtsStatus(TtsStatus newStatus);
    void setTtsOutputFile(const QString &newFilePath);

private slots:
    void onTtsProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onTtsProcessError(QProcess::ProcessError error);

private:
    void cleanupReply();
    void cleanupTtsFile();
    void cleanupTtsProcess();
    void startTtsProcess();

    QPointer<QNetworkReply> _reply;
    QPointer<QProcess> _ttsProcess;
    QString _text;
    bool _done;
    TtsStatus _ttsStatus = TtsStatus::NotStarted;
    QString _ttsOutputFile;
    QString _prompt;
};

#endif // CHATBOTRESPONSE_H
