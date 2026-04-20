import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Clock

import "../Style"
import "../Controls"
import "../Components"

ColumnLayout {
    id: weatherDisplay

    readonly property string currentSymbol: weatherService.currentSymbol
    width: parent.width
    spacing: 16

    WeatherService {
        id: weatherService
    }

    RowLayout {
        Layout.fillWidth: true

        CLabel {
            Layout.fillWidth: true
            text: `Weather in ${weatherService.cityName}...`
            size: CLabel.XLarge
            Layout.bottomMargin: 4
        }
        Item {
            Layout.alignment: Qt.AlignTop

            Icon {
                anchors.right: parent.right
                font.pixelSize: Theme.fontSizeXXLarge
                text: Icons[weatherService.currentSymbol]
            }
        }
    }

    CButton {
        Layout.rightMargin: -16
        Layout.leftMargin: -16
        Layout.fillWidth: true

        rightPadding: 16
        leftPadding: 16
        padding: 0

        bottomInset: 0
        topInset: 0
        flat: true

        onClicked: {
            if (weatherService.status === WeatherService.LocationError)
                weatherService.fetchIPLocation()
            else
                weatherService.fetchWeatherData()
        }

        contentItem: RowLayout {
            width: parent.width
            spacing: 8

            WeatherValueDisplay {
                icon: Icons.device_thermostat
                value: weatherService.temperature
                unit: '°C'
            }
            WeatherValueDisplay {
                value: weatherService.precipitationAmount
                icon: Icons.humidity_high
                unit: 'mm'
            }
            WeatherValueDisplay {
                value: weatherService.windspeed
                unit: 'm/s'
                icon: Icons.air
            }
        }

        Icon {
            anchors.centerIn: parent
            visible: weatherService.WeatherError | weatherService.LocationError
            text: Icons.cloud_off
        }
    }

    RowLayout {
        Layout.alignment: Qt.AlignCenter
        spacing: 8

        CLabel {
            id: hint
            text: `Last update: ${Theme.timeString(weatherService.timestamp)}`
            opacity: Theme.o72
            size: CLabel.Small
        }

        CLabel {
            text: `\u2022`
            opacity: Theme.o72
            size: CLabel.Small
        }
        CLabel {
            id: status

            readonly property string statusString: {
                switch (weatherService.status) {
                case WeatherService.LocationError:
                    return 'Location Error'
                case WeatherService.WeatherError:
                    return 'Weather Error'
                case WeatherService.Fetching:
                    return 'Fetching...'
                case WeatherService.Valid:
                    return 'Valid'
                }
            }

            text: `Status: ${statusString}`
            opacity: Theme.o72
            size: CLabel.Small

            BusyIndicator {
                anchors.left: status.right
                opacity: weatherService.status === WeatherService.Fetching
                height: status.height
            }
        }
    }

    component WeatherValueDisplay: Item {
        id: weatherValueDisplay

        property string value
        property string icon
        property string unit
        property real margins: 8

        implicitHeight: label.implicitHeight
        Layout.fillWidth: true

        Icon {
            anchors.margins: weatherValueDisplay.margins
            anchors.verticalCenterOffset: 2
            anchors.verticalCenter: label.verticalCenter
            anchors.right: label.left

            text: weatherValueDisplay.icon
            font.pixelSize: Theme.fontSizeMedium
        }

        CLabel {
            id: label
            anchors.horizontalCenter: parent.horizontalCenter
            text: weatherValueDisplay.value
            size: CLabel.XXLarge
        }

        CLabel {
            anchors.margins: weatherValueDisplay.margins
            anchors.verticalCenter: label.verticalCenter
            anchors.left: label.right

            text: weatherValueDisplay.unit
            size: CLabel.Medium
        }
    }
}
