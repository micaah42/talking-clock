cmake_minimum_required(VERSION 3.14)

file(GLOB SERVICES *.service)

add_custom_target(systemd
    SOURCES ${SERVICES}
)

install(
    FILES ${SERVICES}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/systemd/system
)
