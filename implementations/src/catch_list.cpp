#include "catch.hpp"
#include "list/list.hpp"
#include <list>

TEST_CASE("list constructor", "[list]") {
	DataStructures::list<int, std::allocator<int>> lst((std::allocator<int>()));

	REQUIRE(lst.size() == 0);
	REQUIRE(lst.empty());
	REQUIRE(lst.begin() == lst.end());
	REQUIRE(lst.rbegin() == lst.rend());
}

/*
Insertion
*/

TEST_CASE("list insert", "[list]") {
	DataStructures::list<int> lst;

	for (int i = 0; i < 10; i++) {
		lst.insert(lst.begin(), 42);
	}
	REQUIRE(lst.size() == 10);
	for (auto& val : lst) {
		REQUIRE(val == 42);
	}
}

TEST_CASE("list insert fill", "[list]") {
	DataStructures::list<int> lst;

	auto it = lst.insert(lst.end(), 10, 42);
	REQUIRE(it == lst.begin());
	REQUIRE(lst.size() == 10);
	for (auto val : lst) {
		REQUIRE(val == 42);
	}
}

TEST_CASE("list initializer list", "[list]") {
	DataStructures::list<int> lst;

	std::initializer_list<int> init_lst = {1, 2, 3, 4, 5};

	auto it = lst.insert(lst.end(), init_lst);
	REQUIRE(lst.size() == init_lst.size());
	REQUIRE(std::equal(it, lst.end(), init_lst.begin()));
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
