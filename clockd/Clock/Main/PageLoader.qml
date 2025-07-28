import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Clock
import "../Style"
import "../Controls"

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

        FeedbackLoader {
            Layout.fillHeight: true
            Layout.fillWidth: true

            sourceComponent: currentPage?.comp || null
        }
    }
}
