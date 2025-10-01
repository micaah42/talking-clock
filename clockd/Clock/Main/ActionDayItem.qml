import QtQuick 2.15
import QtQuick.Layouts

import Clock

import "../Style"
import "../Controls"
import "../Components"

Item {
    property real contentHeight: column.implicitHeight

    Flickable {
        anchors.fill: parent
        contentHeight: column.implicitHeight

        ColumnLayout {
            id: column
            width: parent.width
            spacing: 16

            CLabel {
                Layout.bottomMargin: 4
                size: CLabel.XLarge
                text: `Today is...`
            }

            Repeater {
                model: ActionDayService.days

                delegate: SimpleListItem {
                    Layout.fillWidth: true
                    property ActionDay actionDay: modelData
                    icon: actionDay.icon
                    title: actionDay.name
                    description: actionDay.desc
                }
            }
        }

        Icon {
            anchors.bottom: column.bottom
            anchors.right: parent.right
            anchors.margins: -implicitHeight / 4
            font.pixelSize: parent.width
            text: Icons.calendar_month
            opacity: Theme.o11
        }
    }
}
