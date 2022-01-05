#include "list/list.hpp"
#include "Testing/testclass.hpp"
#include "Testing/list_invariant.hpp"
#include <list>
#include <catch2/catch.hpp>

typedef Testing::TestClass<int> TC;

// #define DS std

TEST_CASE("Temporary Testing", "[tmp]") {
	// DS::list<TC> c2{TC(), TC()};
	// DS::list<TC> lst{TC()};

	// lst = c2;

}
