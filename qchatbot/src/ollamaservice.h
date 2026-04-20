#ifndef OLLAMASERVICE_H
#define OLLAMASERVICE_H

#include <QNetworkAccessManager>
#include <QObject>
#include <QQmlEngine>

#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <QVariant>

#include "chatbotresponse.h"
#include "chatbotservice.h"

struct OllamaModelOptions
{
    std::optional<int> seed = std::nullopt;
    std::optional<float> temperature = std::nullopt;
    std::optional<int> top_k = std::nullopt;
    std::optional<float> top_p = std::nullopt;
    std::optional<float> min_p = std::nullopt;
    std::optional<QStringList> stop = std::nullopt;
    std::optional<int> num_ctx = std::nullopt;
    std::optional<int> num_predict = std::nullopt;

    Q_GADGET
};

struct OllamaGenerateRequest
{
    QString model;
    QString prompt;
    QStringList images;

    bool stream = true;

    QString format;
    QString suffix;
    QString system;

    bool raw = false;
    bool think = false;
    QString keep_alive;
    OllamaModelOptions options;

    bool logprobs = false;
    int top_logprobs = 0;

    Q_GADGET
};

class OllamaService : public ChatBotService
{
    Q_OBJECT
    QML_ELEMENT

public:
    static QNetworkRequest ollamaRequest(const QString &endpoint);
    static QJsonDocument parseResponse(const QByteArray &bytes);

    explicit OllamaService(QObject *parent = nullptr);

public slots:
    ChatBotResponse *generate(const QString &prompt, const QString &model, QObject *parent) override;
    void refreshIsAvailable() override;
    void refreshVersion() override;
    void refreshModels() override;
};

#endif // OLLAMASERVICE_H
