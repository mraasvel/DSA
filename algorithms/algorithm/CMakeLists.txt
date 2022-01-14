set(EXEC "a.out")

add_executable("${EXEC}"
	main.cpp
)

# C++ version
set_target_properties("${EXEC}" PROPERTIES
	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS NO
)

# Header files, inclusions
target_include_directories("${EXEC}" PRIVATE "../include")

# Compilation Flags
target_compile_options("${EXEC}" PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_compile_options("${EXEC}" PRIVATE -Wno-unused -Wno-unused-parameter)

# Address sanitizer
if (CMAKE_BUILD_TYPE MATCHES "Debug")
	target_compile_options("${EXEC}" PRIVATE -fsanitize=address)
	target_link_options("${EXEC}" PRIVATE -fsanitize=address)
endif()
