import QtQuick
import QtQuick.Layouts 1.14
import QtQuick.Controls

import Clock

import "../Style"
import "../Controls"

Section {
    title: 'General'
    icon: Icons.home

    CTextField {
        id: textField
        placeholderText: 'Device Name'
        text: 'Talking Clock'
    }

    CTextField {
        placeholderText: 'Timezone'
        text: AlarmService.now.toLocaleString()
        readOnly: true

        MouseArea {
            anchors.fill: parent
            onClicked: timezoneDialog.open()
        }
    }

    Item {
        Layout.fillHeight: true
        Layout.fillWidth: true
    }

    ColumnLayout {
        spacing: -4
        ConfirmButton {
            Layout.preferredWidth: textField.width
            onAccepted: System.restartApplication()

            dialogBodyText: 'Are you sure you want restart the application?'
            dialogTitle: text
            text: 'Restart Application'
        }
        ConfirmButton {
            Layout.preferredWidth: textField.width
            onAccepted: System.powerOff()

            dialogBodyText: 'Are you sure you want to power off the device?'
            dialogTitle: text
            text: 'Power Off'
        }
        ConfirmButton {
            Layout.preferredWidth: textField.width
            onAccepted: System.reboot()
            dialogBodyText: 'Are you sure you want to reboot the device?'
            dialogTitle: text
            text: 'Reboot'
        }
    }

    Dialog {
        id: timezoneDialog

        title: 'Choose a Timezone'
        standardButtons: Dialog.Ok | Dialog.Cancel

        anchors.centerIn: Overlay.overlay
        height: window.contentItem.height - 64
        width: window.contentItem.width - 64
        bottomPadding: 0

        property TimeZone selectedTimeZone: null
        onAccepted: {
            if (selectedTimeZone === TimeZoneModel.systemTimeZone)
                return
            if (selectedTimeZone === null)
                return

            TimeZoneModel.systemTimeZone = selectedTimeZone
            selectedTimeZone = null
        }

        contentItem: ColumnLayout {
            spacing: 0

            CTextField {
                onTextEdited: searchFilterModel.setFilterFixedString(text)
                placeholderText: 'Search Timezone'
                Layout.fillWidth: true
            }

            FeedbackLoader {
                Layout.fillHeight: true
                Layout.fillWidth: true
                active: timezoneDialog.visible
                clip: true

                ListView {
                    anchors.fill: parent
                    anchors.topMargin: 8

                    currentIndex: -1
                    model: TimeZoneSortFilterModel {
                        id: searchFilterModel
                        model: TimeZoneModel
                    }

                    delegate: ItemDelegate {
                        id: d

                        property TimeZone timeZone: modelData
                        highlighted: timeZone === TimeZoneModel.systemTimeZone
                        onClicked: timezoneDialog.selectedTimeZone = timeZone
                        width: ListView.view.width

                        Rectangle {
                            visible: d.timeZone === timezoneDialog.selectedTimeZone
                            color: Theme.accent
                            height: parent.height
                            width: 4
                        }

                        contentItem: RowLayout {
                            width: parent.width

                            ColumnLayout {
                                Layout.fillWidth: true
                                CLabel {
                                    property string mediumName: d.timeZone.displayName(new Date(), TimeZone.ShortName)
                                    property string cityName: (d.timeZone.id.split('/')[1] || '').replace('_', ' ')
                                    text: `${cityName}, ${d.timeZone.territory}  \u2022  ${mediumName}`
                                    Layout.fillWidth: true
                                }
                                CLabel {
                                    Layout.fillWidth: true
                                    property string offset: d.timeZone.displayName(new Date(), TimeZone.OffsetName)
                                    text: `${d.timeZone.id.replace('_', ' ')}  \u2022  ${offset}`
                                    opacity: Theme.o72
                                    size: CLabel.Small
                                }
                            }
                            CLabel {
                                text: d.timeZone.convertDateTime(AlarmService.now).toLocaleString(Locale.ShortFormat)
                                opacity: Theme.o72
                                size: CLabel.Large
                            }
                        }
                    }
                }
            }
        }
    }
}
