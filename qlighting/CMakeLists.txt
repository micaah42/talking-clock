cmake_minimum_required(VERSION 3.22)

project(qlighting LANGUAGES CXX VERSION 2.0)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAXE_CXX_FLAGS} -Wall -Werror")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui)

add_library(${PROJECT_NAME} SHARED
    src/qlighting_global.h

    src/lighting.cpp
    src/lighting.h

    src/lightmode.cpp
    src/lightmode.h

    src/wavinglight.cpp
    src/wavinglight.h

    src/staticlight.cpp
    src/staticlight.h

    src/prettyrandomlight.cpp
    src/prettyrandomlight.h

    src/pulsatinglight.cpp
    src/pulsatinglight.h

    src/pixel.cpp
    src/pixel.h
)


set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION})

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/>
    $<INSTALL_INTERFACE:src/>
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    QLIGHTING_LIBRARY
)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# include(CTest)
#
# if (BUILD_TESTING)
#     add_subdirectory(tests)
# endif()
