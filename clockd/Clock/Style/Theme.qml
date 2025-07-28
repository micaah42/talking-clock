pragma Singleton

import QtQuick 2.15
import QtQuick.Controls.Material

QtObject {


    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *  Color Management                                                *
    *  Utility functions for shades and applying alpha to a color      *
    * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    readonly property int primaryPalette: Material.Indigo
    readonly property int accentPalette: Material.Red

    function primaryColor(shade) {
        return Material.color(primaryPalette, shade)
    }

    function accentColor(shade) {
        return Material.color(accentPalette, shade)
    }

    function alpha(color, alpha) {
        return Qt.rgba(color.r, color.g, color.b, alpha)
    }

    readonly property color primaryDark: primaryColor(Material.Shade600)
    readonly property color primary: primaryColor(Material.ShadeA700)

    readonly property color accentDark: accentColor(Material.Shade600)
    readonly property color accent: accentColor(Material.ShadeA700)
    readonly property color background: '#000000'

    readonly property real o11: 0.11
    readonly property real o24: 0.24
    readonly property real o42: 0.42
    readonly property real o56: 0.56
    readonly property real o72: 0.72


    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *  Font & Size Management                                          *
    *  Utility functions and constants for maintaining constistend     *
    *  heights and font sized                                          *
    * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    readonly property real fontSizeSmall: 18
    readonly property real fontSizeMedium: 22
    readonly property real fontSizeLarge: 26
    readonly property real fontSizeXLarge: 36
    readonly property real fontSizeXXLarge: 48


    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
       Controls
    * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    readonly property real controlWidth: 320


    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *  Display Conversion Helpers                                      *
    *  Utility functions for converting datatypes to strings           *
    * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    function durationString(msecs, format) {
        const secs = Math.round(msecs / 1000)

        let secondsString
        let minutesString
        let hoursString
        let daysString

        switch (format) {
        case Locale.ShortFormat:
            secondsString = 's'
            minutesString = 'm'
            hoursString = 'h'
            daysString = 'd'
            break
        case Locale.MediumFormat:
        default:
            secondsString = 'sec'
            minutesString = 'min'
            hoursString = 'hrs'
            daysString = 'days'
            break
        case Locale.LongFormat:
            secondsString = 'seconds'
            minutesString = 'minutes'
            hoursString = 'hours'
            daysString = 'days'
            break
        }
        if (secs < 60) {
            return `${secs} ${secondsString}`
        }

        const minutes = Math.round(secs / 60)

        if (minutes < 60) {
            return `${minutes} ${minutesString}`
        }

        const hours = Math.round(minutes / 60)

        if (hours < 24) {
            return `${hours} ${hoursString}`
        }

        const days = hours / 24
        return `${days.toFixed(1)} ${daysString}`
    }
}
