# https://github.com/catchorg/Catch2/blob/v2.x/docs/cmake-integration.md

set(EXEC "test.out")

find_package(Catch2 REQUIRED)
target_link_libraries("${EXEC}" Catch2::Catch2)

# This will install it?

# Include(FetchContent)

# FetchContent_Declare(
	# Catch2
	# GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	# GIT_TAG        v2.13.1)

# FetchContent_MakeAvailable(Catch2)

# target_link_libraries(tests Catch2::Catch2)
