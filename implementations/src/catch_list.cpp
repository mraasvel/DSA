#include "list/list.hpp"
#include "is_iterator.hpp"
#include <list>
#include <catch2/catch.hpp>

TEST_CASE("SFINAE", "[sfinae]") {
	REQUIRE(DataStructures::IsIterator<int*>::value == true);
	REQUIRE(DataStructures::IsIterator<const int*>::value == true);
	REQUIRE(DataStructures::IsIterator<int* const>::value == true);
	REQUIRE(DataStructures::IsIterator<const int* const>::value == true);
	REQUIRE(DataStructures::IsIterator<volatile int*>::value == true);
	REQUIRE(DataStructures::IsIterator<int>::value == false);
}

TEST_CASE("list constructor", "[list]") {
	DataStructures::list<int, std::allocator<int>> lst((std::allocator<int>()));

	REQUIRE(lst.size() == 0);
	REQUIRE(lst.empty());
	REQUIRE(lst.begin() == lst.end());
	REQUIRE(lst.rbegin() == lst.rend());
}

TEST_CASE("list fill constructor", "[list]") {
	DataStructures::list<int> lst(100, 42);
	REQUIRE(lst.size() == 100);
	DataStructures::list<int> lst2(1500);
	REQUIRE(lst2.size() == 1500);
}

TEST_CASE("list range constructor", "[list]") {
	const int table[] = {1, 2, 3, 4, 5};
	const size_t size = sizeof(table) / sizeof(table[0]);

	DataStructures::list<int> lst(table, (table + size));
	REQUIRE(lst.size() == size);
	auto it = lst.begin();
	for (size_t i = 0; i < size; i++) {
		REQUIRE(*it++ == table[i]);
	}

	std::list<int> stdlst(table, table + size);
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
