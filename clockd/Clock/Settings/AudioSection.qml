import QtQuick
import QtQuick.Layouts
import QtMultimedia

import Clock
import "../Style"
import "../Controls"

Section {
    title: 'Audio & Sound'
    icon: Icons.volume_up

    CLabel {
        text: 'Alarm Volume'
    }
    LargeSlider {
        Layout.fillWidth: true
        onValueChanged: SoundService.volume = value
        value: SoundService.volume
        stepSize: 0.1
    }
    RowLayout {
        spacing: 8
        CComboBox {
            id: soundPicker
            placeholderText: 'Choose Sound'
            readonly property string sound: SoundService.availableSounds[currentIndex] || ''
            model: SoundService.availableSounds.map(x => SoundService.displayName(x))
            text: SoundService.displayName(sound)
            currentIndex: 0
        }
        CButton {
            Layout.alignment: Qt.AlignBottom
            onClicked: player.playing ? player.stop() : player.play()
            text: player.playing ? Icons.stop : Icons.play_arrow
            font.pixelSize: Theme.fontSizeLarge
            font.family: Icons.fontFamily
        }
        MediaPlayer {
            id: player
            source: soundPicker.sound ? `file://${soundPicker.sound}` : ''
            audioOutput: AudioOutput {
                volume: SoundService.volume
            }
        }
    }
    CLabel {
        text: 'Sound Effects Volume'
    }
    CSwitch {
        Layout.fillWidth: true
        switchItem.onClicked: MaterialSounds.enabled = !MaterialSounds.enabled
        switchItem.checked: MaterialSounds.enabled
        labelText: 'Enabled'
    }
    LargeSlider {
        Layout.fillWidth: true
        onValueChanged: MaterialSounds.volume = value
        value: MaterialSounds.volume
        stepSize: 0.1
    }
    RowLayout {
        Repeater {
            id: repeater
            property var effects: [//@
                MaterialSounds.alertHighIntensity, //@
                MaterialSounds.navigationForwardSelection, //@
                MaterialSounds.uiTapVariant01, //@
                MaterialSounds.stateChangeConfirmUp, //@
                MaterialSounds.notificationAmbient //@
            ]
            property var names: [//@
                "Alert", //@
                "Navigation", //@
                "Standard UI Tap", //@
                "Confirmation", //@
                "Notification" //@
            ]
            model: 5

            delegate: CButton {
                Layout.fillWidth: true
                implicitWidth: 0
                onClicked: repeater.effects[index].play()
                text: repeater.names[index]
            }
        }
    }

    Item {
        Layout.fillHeight: true
    }
}
