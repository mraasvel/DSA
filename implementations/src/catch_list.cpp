#include "catch.hpp"
#include "list/list.hpp"
#include <list>

TEST_CASE("list constructor", "[list]") {
	DataStructures::list<int> lst;

	REQUIRE(lst.size() == 0);
	REQUIRE(lst.empty());
	REQUIRE(lst.begin() == lst.end());
	REQUIRE(lst.rbegin() == lst.rend());
}

TEST_CASE("list const iterator conversion", "[list]") {
	DataStructures::list<int>::iterator x;
	DataStructures::list<int>::const_iterator y = x;
	DataStructures::list<int>::const_iterator z(x);
	DataStructures::list<int>::const_iterator a;
	a = x;
	REQUIRE(y == x);
	REQUIRE(x == y);
	REQUIRE(z == x);
	REQUIRE(a == x);
	REQUIRE(!(x != x));
}
