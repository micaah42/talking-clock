import QtQuick
import QtQuick.Layouts
import QtQuick.Effects

Item {
    id: root

    property color borderColor: 'white'
    property color value: 'red'
    property real borderWidth: 0.75
    property real radius: 12
    property real spacing: 8

    readonly property real h: currentColor.hsvHue
    readonly property real s: currentColor.hsvSaturation
    readonly property real v: currentColor.hsvValue
    readonly property color hueColor: Qt.hsva(h, 1, 1, 1)

    onValueChanged: currentColor = value
    property color currentColor: value

    function clamp(min, max, value) {
        return Math.min(Math.max(min, value), max)
    }
    function hsvClamp(value) {
        return clamp(0, 1 - 1e-9, value)
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: root.spacing
        opacity: enabled ? 1 : 0.16

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: root.spacing

            Rectangle {
                Layout.preferredWidth: 72
                Layout.fillHeight: true
                border.width: borderWidth
                border.color: borderColor
                radius: root.radius
                color: currentColor
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Rectangle {
                    id: horizontalGradient
                    anchors.fill: parent
                    radius: root.radius
                    layer.enabled: true

                    gradient: Gradient {
                        orientation: Gradient.Horizontal
                        GradientStop {
                            position: 0.0
                            color: "#FFFFFF"
                        }
                        GradientStop {
                            position: 1.0
                            color: hueColor
                        }
                    }
                }
                Rectangle {
                    id: verticalGradient
                    anchors.fill: parent
                    border.width: borderWidth
                    border.color: 'white'
                    radius: root.radius

                    gradient: Gradient {
                        GradientStop {
                            position: 0.0
                            color: "#00000000"
                        }
                        GradientStop {
                            position: 1.0
                            color: "#FF000000"
                        }
                    }
                }

                Item {
                    id: coord
                    anchors.fill: parent
                    anchors.margins: 2 * borderWidth
                    visible: false

                    Rectangle {
                        color: borderColor
                        height: parent.height
                        x: s * parent.width
                        width: borderWidth
                    }
                    Rectangle {
                        color: borderColor
                        width: parent.width
                        y: (1 - v) * parent.height
                        height: borderWidth
                    }
                }

                MultiEffect {
                    anchors.fill: parent
                    maskSource: horizontalGradient
                    maskEnabled: true
                    source: coord
                }

                MouseArea {
                    anchors.fill: parent
                    anchors.margins: borderWidth

                    onPositionChanged: {
                        currentColor.hsvSaturation = hsvClamp(mouseX / width)
                        currentColor.hsvValue = hsvClamp(1 - mouseY / height)
                    }
                    onReleased: {
                        currentColor.hsvSaturation = hsvClamp(mouseX / width)
                        currentColor.hsvValue = hsvClamp(1 - mouseY / height)
                    }
                }
            }
        }
        Rectangle {
            Layout.preferredHeight: 40
            Layout.fillWidth: true
            border.width: borderWidth
            border.color: borderColor
            radius: root.radius

            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop {
                    position: 0.0
                    color: "#FF0000"
                }
                GradientStop {
                    position: 0.16
                    color: "#FFFF00"
                }
                GradientStop {
                    position: 0.33
                    color: "#00FF00"
                }
                GradientStop {
                    position: 0.5
                    color: "#00FFFF"
                }
                GradientStop {
                    position: 0.76
                    color: "#0000FF"
                }
                GradientStop {
                    position: 0.85
                    color: "#FF00FF"
                }
                GradientStop {
                    position: 1.0
                    color: "#FF0000"
                }
            }

            Item {
                anchors.fill: parent
                anchors.margins: borderWidth

                Rectangle {
                    color: borderColor
                    height: parent.height
                    x: h * parent.width
                    width: 1.5
                }
            }

            MouseArea {
                anchors.fill: parent
                anchors.margins: borderWidth
                onPositionChanged: currentColor.hsvHue = hsvClamp(mouseX / width)
                onReleased: currentColor.hsvHue = hsvClamp(mouseX / width)
            }
        }
    }
}
