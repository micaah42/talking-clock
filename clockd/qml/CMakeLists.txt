cmake_minimum_required(VERSION 3.14)

function (clockd_add_qml_module NAME)

    qt_add_qml_module(${NAME}
        QML_FILES ${QML_FILES}
        URI Clock.${NAME}
        STATIC
    )
endfunction()

clockd_add_qml_module(Main)
clockd_add_qml_module(Alarms)
clockd_add_qml_module(Lighting)
clockd_add_qml_module(Settings)

clockd_add_qml_module(Components)
clockd_add_qml_module(Controls)
clockd_add_qml_module(Style)
