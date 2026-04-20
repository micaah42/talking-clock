pragma Singleton

import QtQuick
import QtQuick.Layouts

import "../Style"
import "../Alarms"
import "../Controls"

QtObject {
    id: root
    // component SideBarSection: QtObject {
    //     property Component item
    //     property string icon
    //     property string name
    // }

    // component SideBarSections: QtObject {
    //     default property list<SideBarSection> sections
    // }
    Component.onCompleted: console.log('@', root, availableSideBarSections)

    property var availableSideBarSections: []
    // SideBarSections {
    //     SideBarSection {
    //         id: sec
    //         Component.onCompleted: console.log('@', sec)
    //         icon: Icons.alarm_on
    //         name: 'Active Alarms'
    //         item: ColumnLayout {
    //             width: parent.width
    //             spacing: 16

    //             CLabel {
    //                 Layout.bottomMargin: 4
    //                 size: CLabel.XLarge
    //                 text: `Active Alarms:`
    //             }

    //             Repeater {
    //                 id: repeater
    //                 model: activeAlarms
    //                 delegate: AlarmNotification {
    //                     Layout.fillWidth: true
    //                     alarm: modelData
    //                 }
    //             }
    //         }
    //     }
    //     SideBarSection {
    //         icon: Icons.alarm_on
    //         item: ChatbotItem {}
    //         name: 'ChatBot'
    //     }
    //     SideBarSection {
    //         icon: Icons.weather_mix
    //         item: WeatherItem {}
    //         name: 'Weather'
    //     }
    //     SideBarSection {
    //         icon: Icons.calendar_today
    //         item: ActionDayItem {}
    //         name: 'Action Days'
    //     }
    //     SideBarSection {
    //         icon: Icons.alarm
    //         item: NextAlarm {}
    //         name: 'Next Alarm'
    //     }
    // }
}
