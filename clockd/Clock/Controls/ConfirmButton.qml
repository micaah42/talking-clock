import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material

import "../Controls"

Button {
    id: root

    property alias dialogTitle: dialog.title
    property alias dialogBodyText: dialogBody.text
    property alias dialogBody: dialogBody
    property alias dialog: dialog

    onClicked: dialog.open()
    signal accepted

    Dialog {
        id: dialog
        anchors.centerIn: Overlay.overlay
        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: root.accepted()

        contentItem: CLabel {
            id: dialogBody
        }
    }
}
