cmake_minimum_required(VERSION 3.0)

project("Algorithms")

# C++ Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compilation Flags
if (MSVC)
	add_compile_options(/W3 /WX)
else()
	add_compile_options(-Wall -Wextra -Wpedantic -Werror)
	add_compile_options(-Wno-unused -Wno-unused-parameter)
endif()

# Address sanitizer
if (CMAKE_BUILD_TYPE MATCHES "Debug")
	add_compile_options(-fsanitize=address)
	add_link_options(-fsanitize=address)
endif()

add_subdirectory(datastructures)
add_subdirectory(test)
# add_subdirectory(gtest)
