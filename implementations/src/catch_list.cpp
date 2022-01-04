#include "list/list.hpp"
#include "is_iterator.hpp"
#include "Testing/missing.hpp"
#include <list>
#include <catch2/catch.hpp>

using namespace Testing;

// #define DS std

/*

To use STL `#define DS std` */

TEST_CASE("list default constructor", "[list]") {
	/*
	Class with no constructors, only destructor */
	DS::list<Testing::Nothing>  lst;
	REQUIRE(lst.size() == 0);
	REQUIRE(lst.empty());
	REQUIRE(lst.begin() == lst.end());
	REQUIRE(lst.rbegin() == lst.rend());
}

TEST_CASE("list allocator constructor", "[list]") {
	DS::list<Testing::Nothing> lst((std::allocator<Testing::Nothing>()));
	REQUIRE(lst.get_allocator() == std::allocator<Testing::Nothing>());
	REQUIRE(lst.size() == 0);
	REQUIRE(lst.empty());
	REQUIRE(lst.begin() == lst.end());
	REQUIRE(lst.rbegin() == lst.rend());
}

TEST_CASE("list fill constructor", "[list]") {
	DS::list<Testing::DefaultCopyOnly> lst(42, Testing::DefaultCopyOnly());
	REQUIRE(lst.size() == 42);
	DS::list<Testing::DefaultOnly> lst2(42);
	REQUIRE(lst2.size() == 42);
}

TEST_CASE("list range constructor", "[list]") {
	const int table[] = {1, 2, 3, 4, 5};
	const size_t size = sizeof(table) / sizeof(table[0]);

	DS::list<int> lst(table, (table + size));
	REQUIRE(lst.size() == size);
	auto it = lst.begin();
	for (size_t i = 0; i < size; i++) {
		REQUIRE(*it++ == table[i]);
	}

	std::list<int> stdlst(table, table + size);
}

TEST_CASE("list copy constructor", "[list]") {
	DS::list<int> lst(10, 42);
	DS::list<int> lst2(lst);
	REQUIRE(lst.size() == lst2.size());
	REQUIRE(std::equal(lst.begin(), lst.end(), lst2.begin()));
	REQUIRE(lst.get_allocator() == lst2.get_allocator());
}

TEST_CASE("list allocator copy constructor", "[list]") {
	DS::list<int> lst(10, 42);
	DS::list<int> lst2(lst, std::allocator<int>());
	REQUIRE(lst.size() == lst2.size());
	REQUIRE(std::equal(lst.begin(), lst.end(), lst2.begin()));
	REQUIRE(lst2.get_allocator() == std::allocator<int>());
}

TEST_CASE("list move constructor", "[list]") {
	DS::list<Testing::DefaultOnly> lst((DS::list<Testing::DefaultOnly>(100)));
	REQUIRE(lst.size() == 100);
}

TEST_CASE("list move allocator constructor", "[list]") {
	DS::list<Testing::DefaultCopyOnly> lst((DS::list<Testing::DefaultCopyOnly>(100)), std::allocator<Testing::DefaultCopyOnly>());
	REQUIRE(lst.size() == 100);
}

TEST_CASE("list initializer list constructor", "[list]") {
	DS::list<DCM_ONLY> lst( { DCM_ONLY(), DCM_ONLY() } );
	REQUIRE(lst.size() == 2);
}

/*
Insertion
*/

TEST_CASE("list insert", "[list]") {
	DS::list<int> lst;

	for (int i = 0; i < 10; i++) {
		lst.insert(lst.begin(), 42);
	}
	REQUIRE(lst.size() == 10);
	for (auto& val : lst) {
		REQUIRE(val == 42);
	}
}

TEST_CASE("list insert fill", "[list]") {
	DS::list<int> lst;

	auto it = lst.insert(lst.end(), 10, 42);
	REQUIRE(it == lst.begin());
	REQUIRE(lst.size() == 10);
	for (auto val : lst) {
		REQUIRE(val == 42);
	}
}

TEST_CASE("list initializer list", "[list]") {
	DS::list<int> lst;

	std::initializer_list<int> init_lst = {1, 2, 3, 4, 5};

	auto it = lst.insert(lst.end(), init_lst);
	REQUIRE(lst.size() == init_lst.size());
	REQUIRE(std::equal(it, lst.end(), init_lst.begin()));
}

TEST_CASE("list const iterator conversion", "[list]") {
	DS::list<int>::iterator x;
	DS::list<int>::const_iterator y = x;
	DS::list<int>::const_iterator z(x);
	DS::list<int>::const_iterator a;
	a = x;
	REQUIRE(y == x);
	REQUIRE(x == y);
	REQUIRE(z == x);
	REQUIRE(a == x);
	REQUIRE(!(x != x));
}
