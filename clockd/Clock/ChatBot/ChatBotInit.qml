pragma Singleton

import QtQuick

import Clock
import QChatBot

QtObject {

    id: root

    property ChatBotResponseListModel listModel: ChatBotResponseService.responses
    property LLamaCppService ollamaService: LLamaCppService
    property PromptBuilder promptBuilder: PromptBuilder

    function addPrompt(prompt) {
        listModel.append(ollamaService.generate(prompt, PromptBuilderSettings.model, listModel))
    }

    function addBasicPrompt(extraString) {
        const alarms = AlarmService.nextAlarm ? [AlarmService.nextAlarm] : []
        const days = ActionDayService.days
        const mood = PromptBuilderSettings.mood
        const prompt = promptBuilder.create(alarms, [], days, mood, new Date(), extraString || '')

        addPrompt(prompt)
    }

    default property list<QtObject> data

    property string nextAlarmPrompt: {
        const alarm = AlarmService.nextAlarm
        if (!alarm)
            return ''

        const days = ActionDayService.days
        const mood = PromptBuilderSettings.mood
        return promptBuilder.create([], [alarm], days, mood, alarm.nextTimeout)
    }

    onNextAlarmPromptChanged: {
        console.debug('restarting next alarm response debouncer...')
        responseDebouncer.restart()
    }

    property Timer timer: Timer {
        id: responseDebouncer
        interval: 1500
        onTriggered: {
            if (ActiveAlarmListModel.length) {
                responseDebouncer.restart()
                return
            }

            if (nextAlarmPrompt === '')
                return

            console.info('update next alarm response for', nextAlarmPrompt)
            ChatBotResponseService.alarmResponse = ollamaService.generate(nextAlarmPrompt, PromptBuilderSettings.model,
                                                                          ChatBotResponseService)
        }
    }
}
