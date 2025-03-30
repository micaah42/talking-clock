import QtQuick
import QtQuick.Controls as T
import QtQuick.Controls.Material

T.Dialog {
    id: root
    Material.roundedScale: Material.SmallScale
    modal: true
    dim: true

    header: CLabel {
        leftPadding: root.leftPadding
        topPadding: root.topPadding
        size: CLabel.Large
        text: title
    }
}
