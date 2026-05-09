pragma Singleton

import Clock
import QtQuick
import QLighting

QtObject {
    // Static Light Modes
    property LightMode staticWhite: StaticLight {
        name: "Bright White"
        color: "#FFFFFF"
    }

    property LightMode staticWarm: StaticLight {
        name: "Warm Gold"
        color: "#FFD700"
    }

    property LightMode staticCool: StaticLight {
        name: "Cool Blue"
        color: "#87CEEB"
    }

    property LightMode wavingOcean: WavingLight {
        name: "Ocean Waves"
        a: "#0077BE"
        b: "#00D4FF"
        length: 80
        speed: 0.8
    }

    property LightMode wavingFire: WavingLight {
        name: "Fire Dance"
        a: "#FF6B00"
        b: "#FFD700"
        length: 50
        speed: 1.2
    }

    property LightMode wavingForest: WavingLight {
        name: "Forest Glow"
        a: "#2D5016"
        b: "#90EE90"
        length: 100
        speed: 0.5
    }

    property LightMode monoSunset: MonoRotationLight {
        name: "Sunset Glow"
        gradient: LightingGradient {
            preset: LightingGradient.TrueSunset
        }
        waveForm: MonoRotationLight.Sine
        speed: 1.5
    }

    property LightMode monoNightFade: MonoRotationLight {
        name: "Night Fade"
        gradient: LightingGradient {
            preset: LightingGradient.NightFade
        }
        waveForm: MonoRotationLight.Sine
        speed: 0.8
    }

    property LightMode monoSpringWarmth: MonoRotationLight {
        name: "Spring Warmth"
        gradient: LightingGradient {
            preset: LightingGradient.SpringWarmth
        }
        waveForm: MonoRotationLight.Sine
        speed: 1.0
    }

    property LightMode monoVibes: MonoRotationLight {
        name: "Electric Vibes"
        gradient: LightingGradient {
            preset: LightingGradient.YoungPassion
        }
        waveForm: MonoRotationLight.Sawtooth
        speed: 2.0
    }

    property LightMode monoOcean: MonoRotationLight {
        name: "Deep Ocean"
        gradient: LightingGradient {
            preset: LightingGradient.DeepBlue
        }
        waveForm: MonoRotationLight.Sawtooth
        speed: 1.2
    }

    property LightMode monoBliss: MonoRotationLight {
        name: "Blissful Energy"
        gradient: LightingGradient {
            preset: LightingGradient.HappyAcid
        }
        waveForm: MonoRotationLight.Triangle
        speed: 1.5
    }

    property LightMode monoMystic: MonoRotationLight {
        name: "Mystic Dreams"
        gradient: LightingGradient {
            preset: LightingGradient.MorpheusDen
        }
        waveForm: MonoRotationLight.Triangle
        speed: 0.9
    }

    property LightMode perlinCloud: PerlinLight {
        name: "Cloudy Sky"
        gradient: LightingGradient {
            preset: LightingGradient.CloudyApple
        }
        stretch: 0.01
        speed: 0.5
    }

    property LightMode perlinDream: PerlinLight {
        name: "Frozen Dreams"
        gradient: LightingGradient {
            preset: LightingGradient.FrozenDreams
        }
        stretch: 0.025
        speed: 1.0
    }

    property LightMode perlinMelody: PerlinLight {
        name: "Sweet Melody"
        gradient: LightingGradient {
            preset: LightingGradient.SweetDessert
        }
        stretch: 0.15
        speed: 0.8
    }

    property var modes: [//@
        staticWhite, //@
        staticWarm, //@
        staticCool, //@
        wavingOcean, //@
        wavingFire, //@
        wavingForest, //@
        monoSunset, //@
        monoNightFade, //@
        monoSpringWarmth, //@
        monoVibes, //@
        monoOcean, //@
        monoBliss, //@
        monoMystic, //@
        perlinCloud, //@
        perlinDream, //@
        perlinMelody //@
    ]

    property LightMode mode: modes[0]

    onModeChanged: {
        Lighting.mode = mode
    }

    function init() {
        const singletons = [About, ActionDayService, AlarmService, SoundService, TimeZoneModel, System, SystemLightManager]
        Lighting.mode = mode
    }
}
