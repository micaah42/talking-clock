import QtQuick
import QtQuick.Layouts

Item {
    property color value
    signal valueChanged(var newValue)

    readonly property real hue: 0

    property real borderWidth: 1.5
    property real radius: 8

    RowLayout {
        anchors.fill: parent

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            Rectangle {
                x: r
                y: r + parent.height - 2 * r
                rotation: -90
                transformOrigin: Item.TopLeft
                width: parent.height - 2 * r
                height: parent.width - 2 * r
                gradient: Gradient {
                    GradientStop {
                        position: 0.0
                        color: "#FFFFFF"
                    }
                    GradientStop {
                        position: 1.0
                        color: root.hueColor
                    }
                }
            }
            Rectangle {
                x: r
                y: r
                width: parent.width - 2 * r
                height: parent.height - 2 * r
                gradient: Gradient {
                    GradientStop {
                        position: 1.0
                        color: "#FF000000"
                    }
                    GradientStop {
                        position: 0.0
                        color: "#00000000"
                    }
                }
            }
        }
    }
}
