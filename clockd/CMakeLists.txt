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


qt_standard_project_setup(REQUIRES 6.5)

set(PROJECT_SOURCES
    fonts/fonts.qrc
    icons/icons.qrc
    src/main.cpp
)

file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/*.h
)
list(REMOVE_ITEM SOURCES src/main.cpp)


add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

file(GLOB_RECURSE QML_FILES
    RELATIVE ${CMAKE_CURRENT_LIST_DIR}
    Clock/*.qml
)

set(QML_SINGLETONS
    Clock/Style/Icons.qml
    Clock/Style/Theme.qml
    Clock/Style/Palettes.qml
)

set_source_files_properties(${QML_SINGLETONS}
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(${PROJECT_NAME}
    DEPENDENCIES QtQuick.Controls
    QML_FILES ${QML_FILES}
    SOURCES ${SOURCES}
    URI Clock
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::QuickControls2
    Qt${QT_VERSION_MAJOR}::Sql
    networkmanager-qmlplugin
    qlightingplugin
    qperformance
    qopenremote
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
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    BUNDLE DESTINATION .
)

execute_process(COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/usr)
execute_process(COMMAND cp -r ${CMAKE_CURRENT_LIST_DIR}/share ${CMAKE_CURRENT_BINARY_DIR}/usr)
install(DIRECTORY share/ DESTINATION share/)
