import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import Clock
import QLighting

import "../Style"

ColumnLayout {
    property LightingGradient lightingGradient

    function updateGradientStops() {
        const gradientStops = []
        for (var i = 0; i < repeater.count; i++) {
            gradientStops.push({
                                   "position": repeater.itemAt(i).x / markers.width,
                                   "color": repeater.itemAt(i).color
                               })
        }

        lightingGradient.gradientStops = gradientStops
    }

    Item {
        id: markers
        Layout.preferredHeight: 48
        Layout.fillWidth: true

        Repeater {
            id: repeater
            model: lightingGradient.gradientStops
            delegate: Item {
                id: delegate
                x: modelData.position * markers.width
                height: markers.height
                property color color: modelData.color
                property int i: index

                Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: markers.height - width / 2
                    color: modelData.color
                    transformOrigin: Item.Center
                    scale: mouseArea.pressed ? 1.1 : 1
                    Behavior on scale {
                        PropertyAnimation {}
                    }

                    width: 24

                    Rectangle {
                        id: marker
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.bottom
                        color: modelData.color
                        transformOrigin: Item.Center
                        rotation: 45

                        // 24^2 = 2 * x^2
                        width: Math.sqrt(24 * 24 / 2)
                        height: width
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        drag.target: delegate
                        drag.minimumY: 0
                        drag.maximumY: 0
                        drag.minimumX: {
                            if (delegate.i === 0)
                                return 0
                            return repeater.itemAt(delegate.i - 1).x + marker.width
                        }
                        drag.maximumX: {
                            if (delegate.i === repeater.count - 1)
                                return markers.width
                            return repeater.itemAt(delegate.i + 1).x - marker.width
                        }
                        onReleased: updateGradientStops()
                    }
                }
            }
        }
    }
    Rectangle {
        Layout.preferredHeight: 48
        Layout.fillWidth: true
        border.color: Material.foreground
        radius: 12

        Component {
            id: gradientStop
            GradientStop {}
        }

        gradient: Gradient {
            id: gradient
            orientation: Gradient.Horizontal
            stops: {
                const model = Array.from(lightingGradient.gradientStops)
                return model.map(stop => gradientStop.createObject(this, stop))
            }
        }
    }
}
