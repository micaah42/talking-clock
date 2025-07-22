import QtQuick 2.15
import QtQuick.Layouts

import Clock 1.0
import Clock.Style
import Clock.Controls

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
                model: ActionDayManager.days

                delegate: RowLayout {
                    spacing: 24

                    Icon {
                        font.pixelSize: Theme.fontSizeXLarge
                        text: Icons[modelData.icon] || Icons.calendar_today
                    }

                    ColumnLayout {
                        spacing: 0

                        CLabel {
                            Layout.fillWidth: true
                            elide: Text.ElideRight
                            text: modelData.name
                            size: CLabel.Large
                        }
                        CLabel {
                            Layout.fillWidth: true
                            elide: Text.ElideRight
                            text: modelData.desc
                            size: CLabel.Medium
                            opacity: Theme.o72
                        }
                    }
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
