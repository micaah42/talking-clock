cmake_minimum_required(VERSION 3.22)

project(qperformance LANGUAGES CXX VERSION 2.0)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAXE_CXX_FLAGS} -Wall -Werror")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

find_package(Qt6 REQUIRED COMPONENTS Quick)

add_library(${PROJECT_NAME} SHARED)

qt6_add_qml_module(${PROJECT_NAME}
  URI QPerformance
  VERSION 1.0
  SOURCES
  src/qperformance_global.h
  src/cpumonitor.cpp
  src/cpumonitor.h
  src/monitor.cpp
  src/monitor.h
  src/performancechart.cpp
  src/performancechart.h
)


target_link_libraries(qperformance PRIVATE Qt6::Quick)


set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION})

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/>
    $<INSTALL_INTERFACE:src/>
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    QPERFORMANCE_LIBRARY
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
