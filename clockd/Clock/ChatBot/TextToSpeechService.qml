pragma Singleton

import QtQuick
import QtTextToSpeech

TextToSpeech {
    // QtObject {
    id: root
    Component.onCompleted: {
        console.log(root, 'engines:', availableEngines())
        console.log(root, 'voices:', availableVoices())
    }

    // function say(text) {}
}
