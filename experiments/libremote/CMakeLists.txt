cmake_minimum_required(VERSION 3.14)

project(libremote LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core WebSockets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core WebSockets)

add_library(${PROJECT_NAME} SHARED
  libremote_global.h

  serializer.h
  serializer.cpp

  templatemodel.h
  templatemodel.cpp

  remoting.h
  remoting.cpp



)

set_target_properties(${PROJECT_NAME} PROPERTIES
    SOVERSION ${PROJECT_VERSION}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::Core
)

target_compile_definitions(${PROJECT_NAME} PRIVATE LIBREMOTE_LIBRARY)
install(TARGETS ${PROJECT_NAME} LIBRARY)
