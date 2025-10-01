pragma Singleton

import Clock
import QtQuick
import QLighting

QtObject {
    property var modes: [//@
        StaticLight, //@
        WavingLight, //@
        MonoRotationLight, //@
        PerlinLight //@
    ]

    property LightMode mode: modes[Lighting.modeIndex]

    onModeChanged: {
        Lighting.modeIndex = modes.indexOf(mode)
        Lighting.mode = mode
    }

    function init() {
        const singletons = [About, ActionDayService, AlarmService, SoundService, TimeZoneModel, System]
        Lighting.mode = mode
    }
}
