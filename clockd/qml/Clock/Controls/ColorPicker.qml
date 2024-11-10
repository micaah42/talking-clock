import QtQuick
import QtQuick.Layouts
import QtQuick.Effects

Item {
    id: root

    property color value: 'red'

    property color borderColor: 'white'
    property real borderWidth: 2.5
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

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: root.spacing

            Rectangle {
                Layout.preferredWidth: height
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
                    anchors.fill: parent
                    radius: root.radius

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
                    anchors.fill: parent
                    anchors.margins: borderWidth
                    Rectangle {
                        color: borderColor
                        height: parent.height
                        x: s * parent.width
                        width: 1
                    }
                    Rectangle {
                        color: borderColor
                        width: parent.width
                        y: (1 - v) * parent.height
                        height: 1
                    }
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
