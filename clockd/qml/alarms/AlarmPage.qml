import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14

import ".."
import Controls 1.0

Item {
    ListView {
        id: view
        anchors.margins: 5
        anchors.fill: parent
        spacing: 5
        clip: true

        model: alarms.model
        delegate: AlarmItem {}
        footer: Button {
            height: 50
            width: parent.width
            text: "New Alarm"
            onClicked: {
                alarms.model.push()
            }
            font.family: fontService.family
        }
    }
}
