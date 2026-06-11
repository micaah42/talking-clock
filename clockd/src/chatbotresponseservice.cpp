#include "chatbotresponseservice.h"

ChatBotResponseService::ChatBotResponseService()
    : _responses{new ChatBotResponseListModel(this)}
{}

ChatBotResponseListModel *ChatBotResponseService::responses() const
{
    return _responses;
}

ChatBotResponse *ChatBotResponseService::alarmResponse() const
{
    return _alarmResponse;
}

void ChatBotResponseService::setAlarmResponse(ChatBotResponse *newAlarmResponse)
{
    if (_alarmResponse == newAlarmResponse)
        return;

    _alarmResponse = newAlarmResponse;
    emit alarmResponseChanged();
}
