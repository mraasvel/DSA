set(LIBNAME "dsa")

add_library("${LIBNAME}" STATIC
	heap/heap.cpp
)

target_include_directories("${LIBNAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
