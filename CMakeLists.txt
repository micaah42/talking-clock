cmake_minimum_required(VERSION 3.14)

project(talking-clock VERSION 0.3)

add_subdirectory(web/)

add_subdirectory(clockd/)
add_subdirectory(libremote/)
# add_subdirectory(experiments/clock-test/)
# add_subdirectory(experiments/advanced-modeling/)

install(DIRECTORY web/dist/web/ DESTINATION share/apache2/default-site/htdocs)

set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY NO)
set(CPACK_SOURCE_IGNORE_FILES
  \\.git/
  build/
  ".*~$"
)
set(CPACK_VERBATIM_VARIABLES YES)
include(CPack)
