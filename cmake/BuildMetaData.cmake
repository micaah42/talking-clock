cmake_minimum_required(VERSION 3.26)

function(generate_build_metadata TARGET_NAME TEMPLATE_FILE OUTPUT_FILE)
    string(TIMESTAMP BUILD_TIMESTAMP "%Y-%m-%d %H:%M:%S")

    execute_process(
        COMMAND git rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    if(NOT GIT_COMMIT_HASH)
        set(GIT_COMMIT_HASH "unknown")
    endif()

    configure_file(${TEMPLATE_FILE} ${OUTPUT_FILE} @ONLY)

    target_include_directories(${TARGET_NAME} PUBLIC 
        ${CMAKE_CURRENT_BINARY_DIR}
    )
endfunction()