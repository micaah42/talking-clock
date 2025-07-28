cmake_minimum_required(VERSION 3.14)

project(talking-clock VERSION 0.5)

add_subdirectory(clockd/)
add_subdirectory(qlighting/)
add_subdirectory(qopenremote/)
add_subdirectory(qperformance/)
add_subdirectory(networkmanager-qml/)
add_subdirectory(tools/)

option(TC_SYSTEMD "Install systemd services" ON)

if(TC_SYSTEMD)
    add_subdirectory(systemd/)
endif()

set(CPACK_SOURCE_IGNORE_FILES  \\.git/ build/ ".*~$")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY NO)
set(CPACK_VERBATIM_VARIABLES YES)
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_GENERATOR "TGZ")

include(CPack)
