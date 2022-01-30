set(LIBNAME "dsa")

add_library("${LIBNAME}" INTERFACE)
target_include_directories("${LIBNAME}" INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
