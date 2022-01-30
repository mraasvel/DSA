# https://github.com/catchorg/Catch2/blob/v2.x/docs/cmake-integration.md

set(EXEC "test.out")

add_executable("${EXEC}"
	catch_main.cpp
	catch_heap.cpp
	catch_list.cpp
	catch_red_black_tree_tests.cpp
	catch_temp.cpp
	Testing/missing.cpp
)

target_link_libraries("${EXEC}" PUBLIC "dsa")

Include(FetchContent)
FetchContent_Declare(
	Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG        v2.13.1
)
FetchContent_MakeAvailable(Catch2)

target_link_libraries("${EXEC}" PUBLIC Catch2::Catch2)
