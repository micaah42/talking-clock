#ifndef LAMACPPSERVICE_H
#define LAMACPPSERVICE_H

#include <QObject>
#include <QQmlEngine>

#include <QNetworkRequest>

#include "chatbotresponse.h"
#include "chatbotservice.h"

class LLamaCppService : public ChatBotService
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    static QNetworkRequest llamaCppRequest(const QString &endpoint);
    static QJsonDocument parseResponse(const QByteArray &bytes);

    explicit LLamaCppService(QObject *parent = nullptr);

public slots:
    virtual ChatBotResponse *generate(const QString &prompt, const QString &model, QObject *parent) override;
    virtual void refreshIsAvailable() override;
    virtual void refreshVersion() override;
    virtual void refreshModels() override;
};

#endif // LAMACPPSERVICE_H
