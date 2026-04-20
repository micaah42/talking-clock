#include "chatbotresponse.h"

#include "ollamaservice.h"

#include <QLoggingCategory>
#include <QNetworkReply>

namespace {
Q_LOGGING_CATEGORY(self, "chatbotresponse")
}

ChatBotResponse::ChatBotResponse(QNetworkReply *reply, QObject *parent)
    : QObject{parent}
    , _reply{reply}
    , _done{false}
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
