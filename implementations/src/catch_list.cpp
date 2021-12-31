#include "catch.hpp"
#include "list/list.hpp"

TEST_CASE("list const iterator conversion", "[list]") {
	DataStructures::list<int>::iterator x;
	DataStructures::list<int>::const_iterator y = x;
	DataStructures::list<int>::const_iterator z(x);
	DataStructures::list<int>::const_iterator a;
	a = x;
	REQUIRE(y == x);
	REQUIRE(z == x);
	REQUIRE(a == x);
	REQUIRE(!(x != x));
}
