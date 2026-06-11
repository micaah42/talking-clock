pragma Singleton

import Clock
import QtQuick
import QLighting

QtObject {
    // Custom Gradient Definitions
    property LightingGradient rainbowGradient: LightingGradient {
        gradientStops: [
            {"position": 0.0, "color": "#FF0000"},
            {"position": 0.2, "color": "#FF7F00"},
            {"position": 0.4, "color": "#FFFF00"},
            {"position": 0.6, "color": "#00FF00"},
            {"position": 0.8, "color": "#0000FF"},
            {"position": 1.0, "color": "#9400D3"}
        ]
    }

    property LightingGradient auroraGradient: LightingGradient {
        gradientStops: [
            {"position": 0.0, "color": "#001A4D"},
            {"position": 0.25, "color": "#00FF88"},
            {"position": 0.5, "color": "#00FFFF"},
            {"position": 0.75, "color": "#9D00FF"},
            {"position": 1.0, "color": "#4B0082"}
        ]
    }

    property LightingGradient lavaGradient: LightingGradient {
        gradientStops: [
            {"position": 0.0, "color": "#000000"},
            {"position": 0.25, "color": "#4B0000"},
            {"position": 0.5, "color": "#FF0000"},
            {"position": 0.75, "color": "#FF6600"},
            {"position": 1.0, "color": "#FFFF00"}
        ]
    }

    property LightingGradient twilightGradient: LightingGradient {
        gradientStops: [
            {"position": 0.0, "color": "#1A0033"},
            {"position": 0.25, "color": "#5D0066"},
            {"position": 0.5, "color": "#CC1166"},
            {"position": 0.75, "color": "#FF6B35"},
            {"position": 1.0, "color": "#FF8C00"}
        ]
    }

    property LightingGradient cyberpunkGradient: LightingGradient {
        gradientStops: [
            {"position": 0.0, "color": "#FF00FF"},
            {"position": 0.33, "color": "#00FFFF"},
            {"position": 0.66, "color": "#FFFF00"},
            {"position": 1.0, "color": "#FF00FF"}
        ]
    }

    property LightingGradient forestGradient: LightingGradient {
        gradientStops: [
            {"position": 0.0, "color": "#001A00"},
            {"position": 0.33, "color": "#004D00"},
            {"position": 0.66, "color": "#00FF00"},
            {"position": 1.0, "color": "#FFFF00"}
        ]
    }

    property LightingGradient oceanGradient: LightingGradient {
        gradientStops: [
            {"position": 0.0, "color": "#000033"},
            {"position": 0.33, "color": "#003366"},
            {"position": 0.66, "color": "#0066FF"},
            {"position": 1.0, "color": "#00FFFF"}
        ]
    }

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

    property LightMode staticRuby: StaticLight {
        name: "Ruby Red"
        color: "#E0115F"
    }

    property LightMode staticEmerald: StaticLight {
        name: "Emerald Green"
        color: "#50C878"
    }

    property LightMode staticAmethyst: StaticLight {
        name: "Amethyst Purple"
        color: "#9966CC"
    }

    property LightMode staticCoral: StaticLight {
        name: "Coral Pink"
        color: "#FF7F7F"
    }

    property LightMode staticTurquoise: StaticLight {
        name: "Turquoise"
        color: "#40E0D0"
    }

    property LightMode staticCrimson: StaticLight {
        name: "Crimson"
        color: "#DC143C"
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

    property LightMode wavingRainbow: WavingLight {
        name: "Rainbow Waves"
        a: "#FF0000"
        b: "#00FF00"
        length: 60
        speed: 1.0
    }

    property LightMode wavingAurora: WavingLight {
        name: "Aurora Waves"
        a: "#00FF88"
        b: "#9D00FF"
        length: 90
        speed: 0.7
    }

    property LightMode wavingTwilight: WavingLight {
        name: "Twilight Waves"
        a: "#CC1166"
        b: "#FF8C00"
        length: 70
        speed: 0.9
    }

    property LightMode wavingCyberpunk: WavingLight {
        name: "Cyberpunk Waves"
        a: "#FF00FF"
        b: "#00FFFF"
        length: 40
        speed: 1.4
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

    property LightMode monoRainbow: MonoRotationLight {
        name: "Rainbow Rotation"
        gradient: rainbowGradient
        waveForm: MonoRotationLight.Sine
        speed: 1.3
    }

    property LightMode monoAurora: MonoRotationLight {
        name: "Aurora Rotation"
        gradient: auroraGradient
        waveForm: MonoRotationLight.Triangle
        speed: 1.0
    }

    property LightMode monoLava: MonoRotationLight {
        name: "Lava Flow"
        gradient: lavaGradient
        waveForm: MonoRotationLight.Sawtooth
        speed: 1.6
    }

    property LightMode monoTwilight: MonoRotationLight {
        name: "Twilight Cycle"
        gradient: twilightGradient
        waveForm: MonoRotationLight.Sine
        speed: 0.7
    }

    property LightMode monoCyberpunk: MonoRotationLight {
        name: "Cyberpunk Pulse"
        gradient: cyberpunkGradient
        waveForm: MonoRotationLight.Square
        speed: 2.2
    }

    property LightMode monoForest: MonoRotationLight {
        name: "Forest Cycle"
        gradient: forestGradient
        waveForm: MonoRotationLight.Sine
        speed: 0.6
    }

    property LightMode monoWave: MonoRotationLight {
        name: "Ocean Cycle"
        gradient: oceanGradient
        waveForm: MonoRotationLight.Triangle
        speed: 1.1
    }

    property LightMode perlinCloud: PerlinLight {
        name: "Cloudy Sky"
        gradient: LightingGradient {
            preset: LightingGradient.CloudyApple
        }
        stretch: 0.01
        speed: 0.5
        persistence: 0.5
        frequency: 2.0
        octaves: 3
    }

    property LightMode perlinDream: PerlinLight {
        name: "Frozen Dreams"
        gradient: LightingGradient {
            preset: LightingGradient.FrozenDreams
        }
        stretch: 0.025
        speed: 1.0
        persistence: 0.6
        frequency: 1.5
        octaves: 4
    }

    property LightMode perlinMelody: PerlinLight {
        name: "Sweet Melody"
        gradient: LightingGradient {
            preset: LightingGradient.SweetDessert
        }
        stretch: 0.15
        speed: 0.8
        persistence: 0.4
        frequency: 3.0
        octaves: 2
    }

    property LightMode perlinRainbow: PerlinLight {
        name: "Rainbow Noise"
        gradient: rainbowGradient
        stretch: 0.05
        speed: 0.9
        persistence: 0.55
        frequency: 2.5
        octaves: 3
    }

    property LightMode perlinAurora: PerlinLight {
        name: "Aurora Noise"
        gradient: auroraGradient
        stretch: 0.08
        speed: 0.6
        persistence: 0.7
        frequency: 1.8
        octaves: 5
    }

    property LightMode perlinLava: PerlinLight {
        name: "Lava Noise"
        gradient: lavaGradient
        stretch: 0.02
        speed: 1.2
        persistence: 0.45
        frequency: 3.5
        octaves: 2
    }

    property LightMode perlinCyberpunk: PerlinLight {
        name: "Cyberpunk Noise"
        gradient: cyberpunkGradient
        stretch: 0.12
        speed: 1.5
        persistence: 0.35
        frequency: 4.0
        octaves: 3
    }

    property LightMode perlinForest: PerlinLight {
        name: "Forest Noise"
        gradient: forestGradient
        stretch: 0.04
        speed: 0.4
        persistence: 0.65
        frequency: 1.2
        octaves: 4
    }

    property LightMode perlinOcean: PerlinLight {
        name: "Ocean Noise"
        gradient: oceanGradient
        stretch: 0.03
        speed: 0.7
        persistence: 0.6
        frequency: 2.0
        octaves: 4
    }

    property LightMode perlin2dFlow: PerlinLight2D {
        name: "Flowing Plasma"
        gradient: LightingGradient {
            preset: LightingGradient.HappyAcid
        }
        stretchX: 0.08
        stretchT: 0.02
        speed: 1.0
        persistence: 0.5
        frequency: 2.0
        octaves: 3
    }

    property LightMode perlin2dTurbulence: PerlinLight2D {
        name: "Turbulent Clouds"
        gradient: LightingGradient {
            preset: LightingGradient.CloudyApple
        }
        stretchX: 0.12
        stretchT: 0.01
        speed: 0.6
        persistence: 0.7
        frequency: 1.5
        octaves: 5
    }

    property LightMode perlin2dNorthernLights: PerlinLight2D {
        name: "Northern Lights"
        gradient: auroraGradient
        stretchX: 0.15
        stretchT: 0.03
        speed: 0.5
        persistence: 0.65
        frequency: 1.2
        octaves: 4
    }

    property LightMode perlin2dElectricStorm: PerlinLight2D {
        name: "Electric Storm"
        gradient: cyberpunkGradient
        stretchX: 0.06
        stretchT: 0.05
        speed: 1.8
        persistence: 0.3
        frequency: 4.0
        octaves: 2
    }

    property LightMode perlin2dMoltenRock: PerlinLight2D {
        name: "Molten Rock"
        gradient: lavaGradient
        stretchX: 0.04
        stretchT: 0.015
        speed: 1.1
        persistence: 0.55
        frequency: 3.0
        octaves: 3
    }

    property LightMode perlin2dForestFire: PerlinLight2D {
        name: "Forest Fire"
        gradient: LightingGradient {
            gradientStops: [
                {"position": 0.0, "color": "#001400"},
                {"position": 0.25, "color": "#2D5016"},
                {"position": 0.5, "color": "#FF6600"},
                {"position": 0.75, "color": "#FFD700"},
                {"position": 1.0, "color": "#FFFF00"}
            ]
        }
        stretchX: 0.1
        stretchT: 0.025
        speed: 1.3
        persistence: 0.6
        frequency: 2.5
        octaves: 4
    }

    property LightMode perlin2dOceanDepth: PerlinLight2D {
        name: "Ocean Depth"
        gradient: oceanGradient
        stretchX: 0.09
        stretchT: 0.008
        speed: 0.4
        persistence: 0.75
        frequency: 1.8
        octaves: 5
    }

    property var modes: [//@
        staticWhite, //@
        staticWarm, //@
        staticCool, //@
        staticRuby, //@
        staticEmerald, //@
        staticAmethyst, //@
        staticCoral, //@
        staticTurquoise, //@
        staticCrimson, //@
        wavingOcean, //@
        wavingFire, //@
        wavingForest, //@
        wavingRainbow, //@
        wavingAurora, //@
        wavingTwilight, //@
        wavingCyberpunk, //@
        monoSunset, //@
        monoNightFade, //@
        monoSpringWarmth, //@
        monoVibes, //@
        monoOcean, //@
        monoBliss, //@
        monoMystic, //@
        monoRainbow, //@
        monoAurora, //@
        monoLava, //@
        monoTwilight, //@
        monoCyberpunk, //@
        monoForest, //@
        monoWave, //@
        perlinCloud, //@
        perlinDream, //@
        perlinMelody, //@
        perlinRainbow, //@
        perlinAurora, //@
        perlinLava, //@
        perlinCyberpunk, //@
        perlinForest, //@
        perlinOcean, //@
        perlin2dFlow, //@
        perlin2dTurbulence, //@
        perlin2dNorthernLights, //@
        perlin2dElectricStorm, //@
        perlin2dMoltenRock, //@
        perlin2dForestFire, //@
        perlin2dOceanDepth //@
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
