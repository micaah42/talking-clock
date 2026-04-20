pragma Singleton

import QtQuick
import QtMultimedia

import Clock

QtObject {
    id: root

    property string soundPath: `file://${PathService.shareFilePath('sounds/material-sounds')}`
    property bool enabled: true
    property real volume: 1.0

    component MaterialSoundEffect: SoundEffect {
        property string materialSoundName
        property real specialModifier: 1

        source: enabled ? `${root.soundPath}/${materialSoundName}.wav` : ''
        volume: root.volume * specialModifier

        onStatusChanged: {
            if (status === SoundEffect.Error) {
                console.warn('error loading material sound:', materialSoundName)
            }
        }
    }

    // Alerts
    readonly property MaterialSoundEffect alertError01: MaterialSoundEffect {
        materialSoundName: "alert_error-01"
    }
    readonly property MaterialSoundEffect alertError02: MaterialSoundEffect {
        materialSoundName: "alert_error-02"
    }
    readonly property MaterialSoundEffect alertError03: MaterialSoundEffect {
        materialSoundName: "alert_error-03"
    }

    readonly property MaterialSoundEffect alertHighIntensity: MaterialSoundEffect {
        materialSoundName: "alert_high-intensity"
    }
    readonly property MaterialSoundEffect alertSimple: MaterialSoundEffect {
        materialSoundName: "alert_simple"
    }

    // Navigation
    readonly property MaterialSoundEffect navigationBackwardSelection: MaterialSoundEffect {
        materialSoundName: "navigation_backward-selection"
    }
    readonly property MaterialSoundEffect navigationBackwardSelectionMinimal: MaterialSoundEffect {
        materialSoundName: "navigation_backward-selection-minimal"
    }
    readonly property MaterialSoundEffect navigationForwardSelection: MaterialSoundEffect {
        materialSoundName: "navigation_forward-selection"
    }
    readonly property MaterialSoundEffect navigationForwardSelectionMinimal: MaterialSoundEffect {
        materialSoundName: "navigation_forward-selection-minimal"
    }
    readonly property MaterialSoundEffect navigationSelectionCompleteCelebration: MaterialSoundEffect {
        materialSoundName: "navigation_selection-complete-celebration"
    }
    readonly property MaterialSoundEffect navigationTransitionLeft: MaterialSoundEffect {
        materialSoundName: "navigation_transition-left"
    }
    readonly property MaterialSoundEffect navigationTransitionRight: MaterialSoundEffect {
        materialSoundName: "navigation_transition-right"
    }
    readonly property MaterialSoundEffect navigationUnavailableSelection: MaterialSoundEffect {
        materialSoundName: "navigation_unavailable-selection"
    }
    readonly property MaterialSoundEffect navigationHoverTap: MaterialSoundEffect {
        materialSoundName: "navigation_hover-tap"
    }
    readonly property MaterialSoundEffect navigationCancel: MaterialSoundEffect {
        materialSoundName: "navigation-cancel"
    }

    // UI Elements
    readonly property MaterialSoundEffect uiLock: MaterialSoundEffect {
        materialSoundName: "ui_lock"
    }
    readonly property MaterialSoundEffect uiUnlock: MaterialSoundEffect {
        materialSoundName: "ui_unlock"
    }
    readonly property MaterialSoundEffect uiTapVariant01: MaterialSoundEffect {
        materialSoundName: "ui_tap-variant-01"
    }
    readonly property MaterialSoundEffect uiTapVariant02: MaterialSoundEffect {
        materialSoundName: "ui_tap-variant-02"
    }
    readonly property MaterialSoundEffect uiTapVariant03: MaterialSoundEffect {
        materialSoundName: "ui_tap-variant-03"
    }
    readonly property MaterialSoundEffect uiTapVariant04: MaterialSoundEffect {
        materialSoundName: "ui_tap-variant-04"
    }
    readonly property MaterialSoundEffect uiCameraShutter: MaterialSoundEffect {
        materialSoundName: "ui_camera-shutter"
    }
    readonly property MaterialSoundEffect uiLoading: MaterialSoundEffect {
        materialSoundName: "ui_loading"
    }
    readonly property MaterialSoundEffect uiRefreshFeed: MaterialSoundEffect {
        materialSoundName: "ui_refresh-feed"
    }

    // Hero / Celebrations
    readonly property MaterialSoundEffect heroDecorativeCelebration01: MaterialSoundEffect {
        materialSoundName: "hero_decorative-celebration-01"
    }
    readonly property MaterialSoundEffect heroDecorativeCelebration02: MaterialSoundEffect {
        materialSoundName: "hero_decorative-celebration-02"
    }
    readonly property MaterialSoundEffect heroDecorativeCelebration03: MaterialSoundEffect {
        materialSoundName: "hero_decorative-celebration-03"
    }
    readonly property MaterialSoundEffect heroSimpleCelebration01: MaterialSoundEffect {
        materialSoundName: "hero_simple-celebration-01"
    }
    readonly property MaterialSoundEffect heroSimpleCelebration02: MaterialSoundEffect {
        materialSoundName: "hero_simple-celebration-02"
    }
    readonly property MaterialSoundEffect heroSimpleCelebration03: MaterialSoundEffect {
        materialSoundName: "hero_simple-celebration-03"
    }

    // State Changes
    readonly property MaterialSoundEffect stateChangeConfirmUp: MaterialSoundEffect {
        materialSoundName: "state-change_confirm-up"
    }
    readonly property MaterialSoundEffect stateChangeConfirmDown: MaterialSoundEffect {
        materialSoundName: "state-change_confirm-down"
    }

    // Notifications
    readonly property MaterialSoundEffect notificationDecorative01: MaterialSoundEffect {
        materialSoundName: "notification_decorative-01"
    }
    readonly property MaterialSoundEffect notificationDecorative02: MaterialSoundEffect {
        materialSoundName: "notification_decorative-02"
    }
    readonly property MaterialSoundEffect notificationSimple01: MaterialSoundEffect {
        materialSoundName: "notification_simple-01"
    }
    readonly property MaterialSoundEffect notificationSimple02: MaterialSoundEffect {
        materialSoundName: "notification_simple-02"
    }
    readonly property MaterialSoundEffect notificationHighIntensity: MaterialSoundEffect {
        materialSoundName: "notification_high-intensity"
    }
    readonly property MaterialSoundEffect notificationAmbient: MaterialSoundEffect {
        materialSoundName: "notification_ambient"
    }

    // Misc
    readonly property MaterialSoundEffect alarmGentle: MaterialSoundEffect {
        materialSoundName: "alarm_gentle"
    }
    readonly property MaterialSoundEffect ringtoneMinimal: MaterialSoundEffect {
        materialSoundName: "ringtone_minimal"
    }

    // Functions for variants
    readonly property list<SoundEffect> alertErrorEffects: [alertError01, alertError02, alertError03]
    readonly property list<SoundEffect> uiTapVariant01Effects: [uiTapVariant01, uiTapVariant02, uiTapVariant03, uiTapVariant04]
    readonly property list<SoundEffect> heroDecorativeCelebration01Effects: [heroDecorativeCelebration01, heroDecorativeCelebration02, heroDecorativeCelebration03]
    readonly property list<SoundEffect> heroSimpleCelebration01Effects: [heroSimpleCelebration01, heroSimpleCelebration02, heroSimpleCelebration03]

    function _playRandom(array) {
        if (!array || array.length === 0) {
            return
        }
        const randomIndex = Math.floor(Math.random() * array.length)
        return array[randomIndex].play()
    }

    function playAlertError() {
        _playRandom(alertErrorEffects)
    }
    function playUiTapVariant() {
        if (tapDebouncer.running)
            return

        _playRandom(uiTapVariant01Effects)
        tapDebouncer.start()
    }
    function playHeroDecorativeCelebration() {
        _playRandom(heroDecorativeCelebration01Effects)
    }
    function playHeroSimpleCelebration() {
        _playRandom(heroSimpleCelebration01Effects)
    }

    property Timer tapDebouncer: Timer {
        id: tapDebouncer
        interval: 100
    }
}
