set(EXEC "gtest.out")

enable_testing()

add_executable("${EXEC}"
	test.cpp
)

include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)


target_link_libraries(
	"${EXEC}"
	gtest_main
	alg
)

include(GoogleTest)
gtest_discover_tests("${EXEC}")
