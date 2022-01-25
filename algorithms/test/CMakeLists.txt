# https://github.com/catchorg/Catch2/blob/v2.x/docs/cmake-integration.md

set(EXEC "test.out")

add_executable("${EXEC}"
	main.cpp
	sort.cpp
	maxsubarray.cpp
)

target_link_libraries("${EXEC}" PUBLIC "alg")

Include(FetchContent)
FetchContent_Declare(
	Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG        v2.13.1
)
FetchContent_MakeAvailable(Catch2)

target_link_libraries("${EXEC}" PUBLIC Catch2::Catch2)
