import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as T
import QtQuick.Controls.Material

import "../Style"

T.Dialog {
    id: root
    T.Material.roundedScale: T.Material.SmallScale
    implicitWidth: Theme.popupWidth
    modal: true
    dim: true

    T.Overlay.modal: Rectangle {
        color: Theme.alpha(T.Material.foreground, Theme.o24)

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true

            onReleased: {
                if (root.closePolicy & Popup.CloseOnReleaseOutside)
                    root.close()
            }
            onPressed: {
                if (root.closePolicy & Popup.CloseOnPressOutside)
                    root.close()
            }
        }
    }

    header: CLabel {
        leftPadding: root.leftPadding
        topPadding: root.topPadding
        size: CLabel.Large
        text: title
    }

    footer: Control {
        bottomPadding: root.bottomPadding
        rightPadding: root.rightPadding

        contentItem: RowLayout {
            spacing: 8

            Item {
                Layout.fillWidth: true
            }
            CButton {
                visible: root.standardButtons & T.Dialog.Cancel
                onClicked: reject()
                text: 'Cancel'
            }
            CButton {
                visible: root.standardButtons & T.Dialog.Ok
                onClicked: accept()
                highlighted: true
                text: 'OK'
            }
        }
    }
}
