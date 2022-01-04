#include "list/list.hpp"
#include "Testing/testclass.hpp"
#include <list>
#include <catch2/catch.hpp>

typedef Testing::TestClass<int> TC;

// #define DS std

TEST_CASE("Temporary Testing", "[tmp]") {
	std::allocator<TC> alloc;
	DS::list<TC> lst(DS::list<TC>(1));
}
