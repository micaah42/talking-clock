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
        listModel.append(ollamaService.generate(prompt, '', listModel))
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
        interval: 15000
        onTriggered: {
            console.info('update next alarm response for', nextAlarmPrompt)
            ChatBotResponseService.alarmResponse = ollamaService.generate(nextAlarmPrompt, '', ChatBotResponseService)
        }
    }

    property Connections connections1: Connections {
        target: AlarmService

        function onAlarmTriggered(_alarm) {
            listModel.append(ChatBotResponseService.alarmResponse)
            ChatBotResponseService.alarmResponse.parent = listModel
            ChatBotResponseService.alarmResponse = null
        }
    }

    property Connections connections2: Connections {
        target: TimeService

        function onNowHoursChanged() {
            if (TimeService.now.getHours() === 12) {
                addBasicPrompt('It\'s 12 o clock now!')
            }
        }

        function onNowMinutesChanged() {
            if (TimeService.now.getHours() === 21 && TimeService.now.getMinutes() === 12) {
                addBasicPrompt('It\'s 2112 now, like in the rush song!')
            }
        }
    }
}
