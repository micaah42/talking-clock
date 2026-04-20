#ifndef CHATBOTSERVICE_H
#define CHATBOTSERVICE_H

#include <QNetworkRequest>
#include <QObject>

#include "chatbotresponse.h"

class ChatBotService : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(QStringList models READ models NOTIFY modelsChanged FINAL)
    Q_PROPERTY(QString version READ version NOTIFY versionChanged FINAL)
    Q_PROPERTY(bool isAvailable READ isAvailable NOTIFY isAvailableChanged FINAL)

public:
    explicit ChatBotService(QObject *parent = nullptr);

    QStringList models() const;
    bool isAvailable() const;
    QString version() const;

public slots:
    virtual ChatBotResponse *generate(const QString &prompt, const QString &model, QObject *parent) = 0;
    virtual void refreshIsAvailable() = 0;
    virtual void refreshVersion() = 0;
    virtual void refreshModels() = 0;

signals:
    void isAvailableChanged();
    void modelsChanged();
    void versionChanged();

protected:
    void connectReplyError(QNetworkReply *reply);
    void setIsAvailable(bool newIsAvailable);
    void setModels(const QStringList &newModels);
    void setVersion(const QString &newVersion);

    QNetworkAccessManager _nam;

private:
    QStringList _models;
    QString _version;
    bool _isAvailable;
};

#endif // CHATBOTSERVICE_H
