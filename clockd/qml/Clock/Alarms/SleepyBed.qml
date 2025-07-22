import QtQuick 

import Clock.Style
import Clock.Controls

Item {
    property int pixelSize: 56

    implicitHeight: 1.3 * bed.implicitHeight
    implicitWidth: bed.implicitWidth

    Icon {
        id: bed
        text: Icons.bed
        font.pixelSize: pixelSize
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -0.1 * height

        Repeater {
            id: zzz
            model: 3
            delegate: Item {
                CLabel {
                    property real range

                    PropertyAnimation on range {
                        loops: Animation.Infinite
                        duration: 8000
                        from: 0
                        to: 1
                    }

                    property real position: {
                        const p = range + modelData / zzz.count
                        if (p < 1)
                            return p
                        else
                            return p - 1
                    }

                    x: bed.width * (position * position + 0.8) * 0.5
                    y: -bed.height * position * 0.5 - (parent.height / 3)

                    opacity: {
                        if (position < 0.2)
                            return 5 * position
                        if (position > 0.8)
                            return 5 - (5 * position)

                        return 1
                    }

                    font.pixelSize: pixelSize * (1 + position * position) / 4
                    rotation: 15 * Math.random()
                    text: 'z'
                }
            }
        }
    }
}
