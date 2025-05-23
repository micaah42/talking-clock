cmake_minimum_required(VERSION 3.14)

project(clockd LANGUAGES CXX VERSION 1.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core WebSockets Quick QuickControls2 Multimedia Sql)

if(FETCHCONTENT_FULLY_DISCONNECTED)
    find_package(TinyOrm 0.38.1 CONFIG REQUIRED)
else()
    include(FetchContent)

    FetchContent_Declare(TinyORM
        GIT_REPOSITORY https://github.com/silverqx/TinyORM.git
        GIT_TAG v0.38.1
    )
    FetchContent_MakeAvailable(TinyORM)
endif()



set(QML_IMPORT_PATH ${CMAKE_CURRENT_LIST_DIR}/qml ${CMAKE_BINARY_DIR}/imports CACHE STRING "" FORCE)

set(PROJECT_SOURCES
    icons/icons.qrc
    fonts/fonts.qrc
    qml/qml.qrc

    src/main.cpp

    src/constants.h
    src/defines.h

    src/alarm.h
    src/alarm.cpp

    src/alarmservice.h
    src/alarmservice.cpp

    src/fontservice.h
    src/fontservice.cpp

    src/eventfilter.h
    src/eventfilter.cpp

    src/loghandling.h
    src/loghandling.cpp

    src/palette.h
    src/palette.cpp

    src/pathservice.h
    src/pathservice.cpp

    src/soundservice.h
    src/soundservice.cpp

    src/actionday.h
    src/actionday.cpp

    src/about.cpp
    src/about.h

    src/spacetheme.cpp
    src/spacetheme.h

    src/cpugraph.cpp
    src/cpugraph.h

    src/clientmanager.cpp
    src/clientmanager.h

    src/system.cpp
    src/system.h

)

if (ANDROID)
    add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})
    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android)
else()
    add_executable(${PROJECT_NAME} ${PROJECT_SOURCES}
        src/model.h src/model.cpp
        src/timezonemodel.h src/timezonemodel.cpp)
endif()



target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::QuickControls2
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::Sql
    networkmanager-qmlplugin
    #qnetworkmanager
    qperformance
    qopenremote
    qlighting
    ws2811
    TinyOrm
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

execute_process(COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/usr)
execute_process(COMMAND cp -r ${CMAKE_CURRENT_LIST_DIR}/share ${CMAKE_CURRENT_BINARY_DIR}/usr)
install(DIRECTORY share/ DESTINATION share/)
