import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Clock
import "../Style"
import "../Controls"

Item {
    property var currentPage
    signal closed

    Card {
        id: pageLoader
        anchors.fill: parent
    }

    MouseArea {
        anchors.fill: parent
        anchors.margins: -16
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 16

        Card {
            Layout.preferredHeight: 48
            Layout.fillWidth: true
            bright: true

            CButton {
                height: parent.height
                width: parent.width / 8
                onClicked: closed()
                bottomInset: 0
                topInset: 0
                flat: true
            }

            Icon {
                anchors.verticalCenter: parent.verticalCenter
                x: 16
                text: Icons.chevron_backward
                font.pixelSize: Theme.fontSizeLarge
            }

            CLabel {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                text: currentPage ? currentPage.name : ''
                font.pixelSize: 24
            }
        }

        FeedbackLoader {
            Layout.fillHeight: true
            Layout.fillWidth: true
            sourceComponent: currentPage?.comp || null
        }
    }
}
