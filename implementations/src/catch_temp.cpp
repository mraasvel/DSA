#include "list/list.hpp"
#include "Testing/testclass.hpp"
#include "Testing/list_invariant.hpp"
#include <list>
#include <catch2/catch.hpp>

typedef Testing::TestClass<int> TC;

#define DS std

TEST_CASE("Temporary Testing", "[tmp]") {
	DS::list<TC> lst;

	TC one(2);
	TC two(69);
	std::cout << "Emplace" << std::endl;
	lst.emplace(lst.end(), 42);
	lst.emplace(lst.end(), one);
	lst.emplace(lst.end(), std::move(two));

}
