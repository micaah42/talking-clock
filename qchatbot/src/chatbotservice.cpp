#include "chatbotservice.h"

#include <QLoggingCategory>

namespace {
Q_LOGGING_CATEGORY(self, "chatbotservice")
}

ChatBotService::ChatBotService(QObject *parent)
    : QObject{parent}
    , _isAvailable{false}
{}

void ChatBotService::connectReplyError(QNetworkReply *reply)
{
    connect(reply, &QNetworkReply::errorOccurred, this, [this](const QNetworkReply::NetworkError &error) {
        qCCritical(self) << this << "error occurred:" << error;
        this->setIsAvailable(false);
    });
}

bool ChatBotService::isAvailable() const
{
    return _isAvailable;
}

void ChatBotService::setIsAvailable(bool newIsAvailable)
{
    if (_isAvailable == newIsAvailable)
        return;
    _isAvailable = newIsAvailable;
    emit isAvailableChanged();
}

QStringList ChatBotService::models() const
{
    return _models;
}

void ChatBotService::setModels(const QStringList &newModels)
{
    if (_models == newModels)
        return;
    _models = newModels;
    emit modelsChanged();
}

QString ChatBotService::version() const
{
    return _version;
}

void ChatBotService::setVersion(const QString &newVersion)
{
    if (_version == newVersion)
        return;

    _version = newVersion;
    emit versionChanged();
}
