import QtQuick
import QtQuick.Controls

import Clock

CTextField {
    id: root
    property alias model: input.model
    property alias currentIndex: input.currentIndex
    signal activated(var index)

    onReleased: input.popup.open()
    onFocusChanged: focus = false
    text: input.displayText
    readOnly: true

    ComboBox {
        id: input
        visible: false

        anchors.fill: root
        displayText: (model[currentIndex] || '').toString()
        background: Item {}

        font.family: FontService.family
        popup.y: root.height + 8
        popup.padding: 8

        popup.background: CFrame {
            backgroundColor: Material.dialogColor
            enabled: false
        }

        delegate: ItemDelegate {
            bottomPadding: 4
            topPadding: 4

            width: input.popup.availableWidth
            font.pixelSize: Theme.fontSizeMedium
            font.family: FontService.family
            text: modelData
        }

        onActivated: function (index) {
            root.activated(index)
        }
    }
}
