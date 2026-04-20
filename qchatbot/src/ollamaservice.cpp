#include "ollamaservice.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QNetworkReply>
#include <QRegularExpression>

namespace {
Q_LOGGING_CATEGORY(self, "ollamaservice")
}

QNetworkRequest OllamaService::ollamaRequest(const QString &endpoint)
{
    QNetworkRequest request{QUrl{QString{"http://127.0.0.1:11434/api/%1"}.arg(endpoint)}};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    return request;
}

OllamaService::OllamaService(QObject *parent)
    : ChatBotService{parent}
{
    OllamaService::refreshVersion();
    OllamaService::refreshModels();
}

ChatBotResponse *OllamaService::generate(const QString &prompt, const QString &model, QObject *parent)
{
    OllamaGenerateRequest generateRequest;
    generateRequest.prompt = prompt;
    generateRequest.model = model;
    generateRequest.keep_alive = "10m";
    generateRequest.options.num_predict = 256;
    generateRequest.options.temperature = 1.25;

    QJsonObject requestJson{
        {"model", generateRequest.model},
        {"prompt", generateRequest.prompt},
        {"images", QJsonArray::fromStringList(generateRequest.images)},
        {"stream", generateRequest.stream},
        {"format", generateRequest.format},
        {"suffix", generateRequest.suffix},
        {"system", generateRequest.system},
        {"raw", generateRequest.raw},
        {"think", generateRequest.think},
        {"keep_alive", generateRequest.keep_alive},
        {"options",
         QJsonObject{
             // {"seed", generateRequest.options.seed.value_or(QJsonValue::Undefined)},
             {"temperature", generateRequest.options.temperature.value_or(QJsonValue::Undefined)},
             // {"top_k", generateRequest.options.top_k.value_or(QJsonValue::Undefined)},
             // {"top_p", generateRequest.options.top_p.value_or(QJsonValue::Undefined)},
             // {"min_p", generateRequest.options.min_p.value_or(QJsonValue::Undefined)},
             // {"stop", QJsonArray::fromStringList(generateRequest.options.stop.value_or(QStringList()))},
             // {"num_ctx", generateRequest.options.num_ctx.value_or(QJsonValue::Undefined)},
             // {"num_predict", generateRequest.options.num_predict.value_or(QJsonValue::Undefined)},
         }},
        {"logprobs", generateRequest.logprobs},
        {"top_logprobs", generateRequest.top_logprobs},
    };

    auto request = OllamaService::ollamaRequest("generate");
    qCInfo(self) << "posting generate request:" << request.url() << requestJson;

    auto reply = _nam.post(request, QJsonDocument{requestJson}.toJson());
    this->connectReplyError(reply);
    reply->setParent(parent);

    auto chatBotResponse = new ChatBotResponse{reply, parent};

    connect(reply, &QNetworkReply::readyRead, chatBotResponse, [reply, chatBotResponse]() {
        while (reply->canReadLine()) {
            const auto bytes = reply->readLine();
            qCDebug(self) << "received chunk:" << bytes;
            auto object = OllamaService::parseResponse(bytes).object();

            auto error = object["error"];
            if (error.isString()) {
                const auto errorString = error.toString();
                qCWarning(self) << "received error:" << errorString;
                emit chatBotResponse->errorOccurred(error.toString());
            }

            auto response = object["response"].toString();
            chatBotResponse->setText(chatBotResponse->text() + response);
            emit chatBotResponse->textReceived(response);

            auto done = object["done"].toBool();
            chatBotResponse->setDone(done);
        }
    });

    return chatBotResponse;
}

void OllamaService::refreshIsAvailable()
{
    this->refreshVersion();
}

void OllamaService::refreshVersion()
{
    auto request = OllamaService::ollamaRequest("version");
    auto reply = _nam.get(request);
    this->connectReplyError(reply);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        auto object = OllamaService::parseResponse(reply->readAll()).object();
        this->setVersion(object["version"].toString());
        this->setIsAvailable(true);

        reply->deleteLater();
    });
}

void OllamaService::refreshModels()
{
    auto request = OllamaService::ollamaRequest("tags");
    auto reply = _nam.get(request);
    this->connectReplyError(reply);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        const auto object = OllamaService::parseResponse(reply->readAll()).object();
        const auto models = object["models"].toArray();
        this->setIsAvailable(true);

        QStringList modelStrings;
        for (auto const &model : models) {
            const auto modelObject = model.toObject();
            modelStrings.append(modelObject["model"].toString());
        }

        this->setModels(modelStrings);
        reply->deleteLater();
    });
}


QJsonDocument OllamaService::parseResponse(const QByteArray &bytes)
{
    QJsonParseError parseError;
    auto const document = QJsonDocument::fromJson(bytes, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qCWarning(self) << "failed to parse response:" << parseError.errorString() << bytes;
        return {};
    }

    return document;
}
