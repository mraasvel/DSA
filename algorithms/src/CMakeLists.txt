set(LIBNAME "alg")

add_library("${LIBNAME}" STATIC
	factorial.cpp
)

target_include_directories("${LIBNAME}" PUBLIC "../include")
