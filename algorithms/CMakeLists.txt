cmake_minimum_required(VERSION 3.11)

project("Algorithms")

# C++ Standard
set(CMAKE_CXX_STANDARD 11 CACHE STRING "C++ Standard")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compilation Flags
add_compile_options(-Wall -Wextra -Wpedantic -Werror)
add_compile_options(-Wno-unused -Wno-unused-parameter)

# Address sanitizer
if (CMAKE_BUILD_TYPE MATCHES "Debug")
	add_compile_options(-fsanitize=address)
	add_link_options(-fsanitize=address)
endif()

add_subdirectory(algorithm)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(gtest)
