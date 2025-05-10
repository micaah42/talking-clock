import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Clock
import Clock.Style
import Clock.Controls

GridLayout {
    property color value: Qt.transparent
    signal valueEdited(var newValue)

    readonly property var currentPalette: Palettes.palettes[currentIndex % Palettes.palettes.length]
    property int currentIndex: 0

    columns: 5
    rows: 3

    Dialog {
        id: customColorPopup
        anchors.centerIn: Overlay.overlay
        height: 7 * window.height / 8
        width: 7 * window.width / 8

        title: 'Custom Color'

        contentItem: ColorPicker {
            id: customColorPicker
            onCurrentColorChanged: valueEdited(currentColor)
            value: value
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }

    RowLayout {
        Layout.columnSpan: parent.columns

        CToolButton {
            size: CToolButton.Large
            onClicked: currentIndex -= 1
            text: Icons.chevron_backward
        }
        CLabel {
            size: CLabel.Large
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
            text: currentPalette.name
        }

        CToolButton {
            size: CToolButton.Large
            onClicked: currentIndex += 1
            text: Icons.chevron_forward
        }
    }

    Repeater {
        model: currentPalette.colors
        delegate: Button {
            Layout.fillHeight: true
            Layout.fillWidth: true
            onClicked: value = modelData
            Material.background: modelData
            implicitHeight: 0
            bottomInset: 0
            topInset: 0
        }
    }

    Button {
        Layout.fillHeight: true
        Layout.fillWidth: true
        onClicked: customColorPopup.open()
        Material.background: customColorPicker.currentColor
        font.family: Icons.fontFamily
        text: Icons.colors
        implicitHeight: 0
        bottomInset: 0
        topInset: 0
    }
}
