import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Clock
import Clock.Style
import Clock.Controls

Card {
    id: pageLoader

    property var currentPage
    signal closed

    MouseArea {
        anchors.fill: parent
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 16

        Card {
            Layout.preferredHeight: 56
            Layout.fillWidth: true
            bright: true

            CToolButton {
                anchors.verticalCenter: parent.verticalCenter
                text: Icons.chevron_backward
                size: CToolButton.Large
                onClicked: closed()
            }

            CLabel {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                text: currentPage ? currentPage.name : ''
                font.pixelSize: 24
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            Loader {
                id: loader
                anchors.fill: parent
                asynchronous: true

                sourceComponent: currentPage?.comp || null
                opacity: status === Loader.Ready ? 1 : 0

                Behavior on opacity {
                    PropertyAnimation {}
                }
            }

            BusyIndicator {
                anchors.centerIn: parent
                opacity: loader.status === Loader.Loading ? 1 : 0

                Behavior on opacity {
                    PropertyAnimation {}
                }
            }
        }
    }
}
