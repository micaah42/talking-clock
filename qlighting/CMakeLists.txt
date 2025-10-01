cmake_minimum_required(VERSION 3.22)

project(qlighting LANGUAGES CXX VERSION 2.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_FLAGS "${CMAXE_CXX_FLAGS} -Wall -Werror")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Quick)

set(SOURCES
    src/qlighting_global.h

    src/lighting.cpp
    src/lighting.h

    src/lightmode.cpp
    src/lightmode.h

    src/wavinglight.cpp
    src/wavinglight.h

    src/staticlight.cpp
    src/staticlight.h

    src/pixel.cpp
    src/pixel.h

    src/lightingdisplay.cpp
    src/lightingdisplay.h

    src/monorotation.cpp
    src/monorotation.h

    src/systemlight.cpp
    src/systemlight.h

    src/gradient.cpp
    src/gradient.h

    src/perlinlight.cpp
    src/perlinlight.h
)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(${PROJECT_NAME} STATIC)

target_link_libraries(qlighting PRIVATE Qt6::Core)

qt_add_qml_module(${PROJECT_NAME}
    SOURCES ${SOURCES}
    URI QLighting
)

set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION})

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/>
    $<INSTALL_INTERFACE:src/>
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    qopenremote
    ws2811
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    QLIGHTING_LIBRARY
)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

add_executable(${PROJECT_NAME}-cli
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}-cli PRIVATE
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    ${PROJECT_NAME}
)

install(TARGETS ${PROJECT_NAME}-cli)

# include(CTest)
#
# if (BUILD_TESTING)
#     add_subdirectory(tests)
# endif()
