cmake_minimum_required(VERSION 3.14)

project(clockd LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core WebSockets Quick QuickControls2 Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core WebSockets Quick QuickControls2 Multimedia)

add_executable(clockd
  src/alarm.h
  src/alarm.cpp

  src/alarmservice.h
  src/alarmservice.cpp

  src/application_defines.h

  src/defines.h
  src/constants.h

  src/fontservice.h
  src/fontservice.cpp

  src/jsonspecification.h
  src/jsonspecification.cpp

  src/loghandling.h
  src/loghandling.cpp

  src/model.h
  src/model.cpp

  src/palette.h
  src/palette.cpp

  src/pathservice.h
  src/pathservice.cpp

  src/payload.h
  src/payload.cpp

  src/settingsservice.h
  src/settingsservice.cpp

  src/soundservice.h
  src/soundservice.cpp

  src/soundplayer.h
  src/soundplayer.cpp

  src/taskservice.h
  src/taskservice.cpp
  src/main.cpp

  qml/qml.qrc
  fonts/fonts.qrc
  sounds/sounds.qrc
)

target_link_libraries(clockd
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::QuickControls2
    Qt${QT_VERSION_MAJOR}::Multimedia
    #Qt${QT_VERSION_MAJOR}::VirtualKeyboard
    libremote
)
include_directories(../libremote/)

install(TARGETS clockd RUNTIME)