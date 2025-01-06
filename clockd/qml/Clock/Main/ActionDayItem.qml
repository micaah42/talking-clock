import QtQuick 2.15
import QtQuick.Layouts

import Clock 1.0
import Clock.Style
import Clock.Controls

Item {
    Flickable {
        anchors.fill: parent
        contentHeight: childrenRect.height

        ColumnLayout {
            width: parent.width
            spacing: 8

            Repeater {
                model: ActionDayManager.days

                delegate: ColumnLayout {
                    spacing: 0

                    CLabel {
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                        text: modelData.name
                        font.pixelSize: 24
                    }
                    CLabel {
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                        text: modelData.desc
                        opacity: 0.72
                    }
                }
            }
        }
    }
}
