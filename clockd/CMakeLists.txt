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

  src/defines.h
  src/constants.h

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

  src/settingsservice.h
  src/settingsservice.cpp

  src/soundservice.h
  src/soundservice.cpp

  src/taskservice.h
  src/taskservice.cpp

  src/dataservice.h
  src/dataservice.cpp

  src/main.cpp

  qml/qml.qrc
  fonts/fonts.qrc
  icons/icons.qrc
  sounds/sounds.qrc
)

set(QML_IMPORT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/qml)

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
install(DIRECTORY sounds/ DESTINATION share/clockd/sounds)
