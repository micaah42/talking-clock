cmake_minimum_required(VERSION 3.14)

project(talking-clock VERSION 0.5)

add_subdirectory(web/)
add_subdirectory(clockd/)
add_subdirectory(qlighting/)
add_subdirectory(qopenremote/)
add_subdirectory(qperformance/)

set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY NO)
set(CPACK_SOURCE_IGNORE_FILES
  \\.git/
  build/
  ".*~$"
)
set(CPACK_VERBATIM_VARIABLES YES)
include(CPack)
