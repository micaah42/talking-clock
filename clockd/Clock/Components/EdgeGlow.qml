import QtQuick
import QtQuick.Controls.Material

import "../Style"

Item {
    id: root

    property color edgeColor: Theme.alpha(Material.foreground, Theme.o56)
    property color insideColor: 'transparent'
    property real glowWidth: 128

    // top
    Rectangle {
        height: root.glowWidth
        width: root.width

        gradient: Gradient {
            GradientStop {
                color: root.edgeColor
            }
            GradientStop {
                color: root.insideColor
                position: 1
            }
        }
    }
    Rectangle {
        anchors.bottom: root.bottom
        height: root.glowWidth
        width: root.width

        gradient: Gradient {
            GradientStop {
                color: root.insideColor
            }
            GradientStop {
                color: root.edgeColor
                position: 1
            }
        }
    }
    Rectangle {
        width: root.glowWidth
        height: root.height

        gradient: Gradient {
            orientation: Gradient.Horizontal
            GradientStop {
                color: root.edgeColor
            }
            GradientStop {
                color: root.insideColor
                position: 1
            }
        }
    }
    Rectangle {
        anchors.right: root.right
        width: root.glowWidth
        height: root.height

        gradient: Gradient {
            orientation: Gradient.Horizontal
            GradientStop {
                color: root.insideColor
            }
            GradientStop {
                color: root.edgeColor
                position: 1
            }
        }
    }
}
