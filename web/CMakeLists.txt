cmake_minimum_required(VERSION 3.14)

add_custom_target(web)
add_custom_command(TARGET web
    PRE_BUILD
    COMMAND ng build ${CMAKE_SOURCE_DIR}
)
