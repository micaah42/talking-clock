#ifndef CHATBOTRESPONSESERVICE_H
#define CHATBOTRESPONSESERVICE_H

#include "chatbotresponse.h"
#include "listmodel.h"

class ChatBotResponseListModel : public ListModel<ChatBotResponse *>
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit ChatBotResponseListModel(QObject *parent = nullptr)
        : ListModel<ChatBotResponse *>(parent)
    {}
};

class ChatBotResponseService : public ChatBotResponseListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    /* global list of responses */
    Q_PROPERTY(ChatBotResponseListModel *responses READ responses CONSTANT FINAL)

    /* this is is the response to the current alarm */
    Q_PROPERTY(ChatBotResponse *alarmResponse READ alarmResponse WRITE setAlarmResponse NOTIFY alarmResponseChanged FINAL)

public:
    explicit ChatBotResponseService();

    ChatBotResponseListModel *responses() const;

    ChatBotResponse *alarmResponse() const;
    void setAlarmResponse(ChatBotResponse *newAlarmResponse);

signals:
    void nextAlarmChanged();
    void alarmResponseChanged();

private:
    ChatBotResponseListModel *_responses = nullptr;
    ChatBotResponse *_alarmResponse = nullptr;
};

#endif // CHATBOTRESPONSESERVICE_H
