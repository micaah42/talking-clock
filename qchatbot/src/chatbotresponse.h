#ifndef CHATBOTRESPONSE_H
#define CHATBOTRESPONSE_H

#include <QNetworkReply>
#include <QObject>
#include <QPointer>
#include <QQmlEngine>

class ChatBotResponse : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(QString text READ text NOTIFY textChanged FINAL)
    Q_PROPERTY(bool done READ done NOTIFY doneChanged FINAL)

public:
    QString text() const;
    bool done() const;

signals:
    void errorOccurred(const QString &errorString);
    void textReceived(const QString &text);
    void textChanged();
    void doneChanged();

protected:
    explicit ChatBotResponse(QNetworkReply *reply, QObject *parent = nullptr);

    friend class LLamaCppService;
    friend class OllamaService;

    void setText(const QString &newText);
    void setDone(bool newDone);

private:
    QPointer<QNetworkReply> _reply;
    QString _text;
    bool _done;
};

#endif // CHATBOTRESPONSE_H
