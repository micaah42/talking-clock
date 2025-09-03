cmake_minimum_required(VERSION 3.16)

project(networkmanager-qml VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_QML_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR})

find_package(Qt6 6.5 COMPONENTS Quick REQUIRED)
find_package(KF6NetworkManagerQt)

qt_add_library(${PROJECT_NAME} STATIC)

qt_add_qml_module(${PROJECT_NAME}
    URI NetworkManagerQml

    SOURCES
    src/networkmanagerqml.cpp
    src/networkmanagerqml.h

    src/wirelessdeviceqml.cpp
    src/wirelessdeviceqml.h

    src/wirelessnetworkqml.cpp
    src/wirelessnetworkqml.h

    src/wirelesssettingqml.cpp
    src/wirelesssettingqml.h

    src/connectionqml.cpp
    src/connectionqml.h

    src/settingqml.cpp
    src/settingqml.h

    src/wirelesssecuritysettingqml.cpp
    src/wirelesssecuritysettingqml.h

    src/dbusreplyqml.cpp
    src/dbusreplyqml.h

    src/activeconnection.cpp
    src/activeconnection.h


)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    KF6::NetworkManagerQt
    Qt6::Quick
    qopenremote
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

