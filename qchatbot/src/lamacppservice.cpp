#include "lamacppservice.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QNetworkReply>

namespace {
Q_LOGGING_CATEGORY(self, "lamacppservice", QtInfoMsg)
}

QNetworkRequest LLamaCppService::llamaCppRequest(const QString &endpoint)
{
    QNetworkRequest request{QUrl{QString{"http://127.0.0.1:8080/%1"}.arg(endpoint)}};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    return request;
}

LLamaCppService::LLamaCppService(QObject *parent)
    : ChatBotService{parent}
{
    LLamaCppService::refreshVersion();
    LLamaCppService::refreshModels();
}

ChatBotResponse *LLamaCppService::generate(const QString &prompt, const QString &model, QObject *parent)
{
    QJsonObject requestJson{
        {"messages",
         QJsonArray{
             QJsonObject{{"content", prompt}, {"role", "user"}},
         }},
        // {"reasoning_format", "auto"},
        // {"timings_per_token", true},
        // {"return_progress", true},
        {"stream", true},
        // {"temperature", 1.0},
        // {"n_predict", 512},
    };

    auto request = LLamaCppService::llamaCppRequest("chat/completions");
    auto bytes = QJsonDocument{requestJson}.toJson();
    qCInfo(self) << "posting generate request:" << request.url() << bytes;

#if 1
    QFile file{"/tmp/request.json"};
    if (file.open(QIODevice::WriteOnly)) {
        file.write(bytes);
        file.close();
    }
#endif

    auto reply = _nam.post(request, bytes);
    this->connectReplyError(reply);
    reply->setParent(parent);

    auto chatBotResponse = new ChatBotResponse{reply, parent};

    connect(reply, &QNetworkReply::readyRead, chatBotResponse, [this, reply, chatBotResponse]() {
        while (reply->canReadLine()) {
            const auto bytes = reply->readLine().trimmed();

            if (bytes.isEmpty())
                continue;

            qCDebug(self) << this << reply << "received chunk:" << bytes;
            auto object = LLamaCppService::parseResponse(bytes).object();

            auto error = object["error"];
            if (error.isString()) {
                const auto errorString = error.toString();
                qCWarning(self) << "received error:" << errorString;
                emit chatBotResponse->errorOccurred(error.toString());
            }

            auto choices = object["choices"].toArray();
            if (choices.empty())
                return;

            auto choice = choices.first().toObject();
            auto response = choice["delta"].toObject()["content"].toString();
            qCDebug(self) << this << reply << "received delta:" << response;

            chatBotResponse->setText(chatBotResponse->text() + response);
            emit chatBotResponse->textReceived(response);
            auto done = choice["finish_reason"].toString() == "stop";
            chatBotResponse->setDone(done);
        }
    });

    return chatBotResponse;
}

void LLamaCppService::refreshIsAvailable()
{
    this->refreshVersion();
}

void LLamaCppService::refreshVersion()
{
    auto request = LLamaCppService::llamaCppRequest("models");
    auto reply = _nam.get(request);
    this->connectReplyError(reply);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        auto object = LLamaCppService::parseResponse(reply->readAll()).object();
        // llama.cpp doesn't provide version via this endpoint, set a default
        this->setVersion("llama.cpp");
        this->setIsAvailable(true);

        reply->deleteLater();
    });
}

void LLamaCppService::refreshModels()
{
    auto request = LLamaCppService::llamaCppRequest("models");
    auto reply = _nam.get(request);
    this->connectReplyError(reply);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        const auto object = LLamaCppService::parseResponse(reply->readAll()).object();
        const auto models = object["data"].toArray();
        this->setIsAvailable(true);

        QStringList modelStrings;
        for (auto const &model : models) {
            const auto modelObject = model.toObject();
            modelStrings.append(modelObject["id"].toString());
        }

        this->setModels(modelStrings);
        reply->deleteLater();
    });
}

QJsonDocument LLamaCppService::parseResponse(const QByteArray &bytes)
{
    static const QByteArray startString("data: ");

    if (!bytes.startsWith(startString)) {
        qCWarning(self) << "expected starts with" << startString;
        return {};
    }

    QJsonParseError parseError;
    auto const document = QJsonDocument::fromJson(bytes.mid(startString.size()), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qCWarning(self) << "failed to parse response:" << parseError.errorString() << bytes;
        return {};
    }

    return document;
}
