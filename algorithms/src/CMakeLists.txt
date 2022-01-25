set(LIBNAME "alg")

add_library("${LIBNAME}" STATIC
	algorithms.cpp
	maximum_subarray.cpp
	sfinae.cpp
	sort.cpp
)

target_include_directories("${LIBNAME}" PUBLIC "../include")
