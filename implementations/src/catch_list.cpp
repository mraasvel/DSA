#include "list/list.hpp"
#include "is_iterator.hpp"
#include "Testing/missing.hpp"
#include "Testing/list_invariant.hpp"
#include <list>
#include <catch2/catch.hpp>

using namespace Testing;

using Type = size_t;
using CompoundType = std::vector<std::vector<int>>;

// #define DS std

template <typename T>
static bool isValidList(const std::list<T>& l) {
	return true;
}

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
	const Type table[] = {1, 2, 3, 4, 5};
	const size_t size = sizeof(table) / sizeof(table[0]);

	DS::list<Type> lst(table, (table + size));
	REQUIRE(lst.size() == size);
	auto it = lst.begin();
	for (size_t i = 0; i < size; i++) {
		REQUIRE(*it++ == table[i]);
	}

	std::list<Type> stdlst(table, table + size);
}

TEST_CASE("list copy constructor", "[list]") {
	DS::list<Type> lst(10, 42);
	DS::list<Type> lst2(lst);
	REQUIRE(lst.size() == lst2.size());
	REQUIRE(std::equal(lst.begin(), lst.end(), lst2.begin()));
	REQUIRE(lst.get_allocator() == lst2.get_allocator());
}

TEST_CASE("list allocator copy constructor", "[list]") {
	DS::list<Type> lst(10, 42);
	DS::list<Type> lst2(lst, std::allocator<Type>());
	REQUIRE(lst.size() == lst2.size());
	REQUIRE(std::equal(lst.begin(), lst.end(), lst2.begin()));
	REQUIRE(lst2.get_allocator() == std::allocator<Type>());
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
Assignation */

TEST_CASE("list basic assign", "[list]") {
	DS::list<Type> orig {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	std::vector<DS::list<Type>> lists {
		DS::list<Type> {},
		DS::list<Type> {1, 2, 3, 4, 5},
		DS::list<Type> {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
	};

	for (auto& c : lists) {
		c = orig;
		REQUIRE(c.size() == orig.size());
		REQUIRE(std::equal(orig.begin(), orig.end(), c.begin()));
		REQUIRE(std::equal(c.begin(), c.end(), orig.begin()));
	}
}

TEST_CASE("list move assign", "[list]") {
	DS::list<Type> lst;

	lst = DS::list<Type> {1, 2, 3, 4, 5};
	REQUIRE(isValidList(lst));
	REQUIRE(lst.size() == 5);
	DS::list<Type> c2{1, 2, 3, 4};
	c2 = DS::list<Type>{1};
	REQUIRE(isValidList(c2));
	REQUIRE(c2.size() == 1);

	c2 = std::move(lst);
	REQUIRE(c2.size() == 5);
	REQUIRE(isValidList(c2));
}

TEST_CASE("list assign fill", "[list]") {
	DS::list<Type> lst;

	lst.assign(42, 100);
	REQUIRE(lst.size() == 42);
	REQUIRE(isValidList(lst));
	lst.assign(0, 1);
	REQUIRE(lst.empty());
	REQUIRE(isValidList(lst));
	lst.assign(1000, 42);
	REQUIRE(lst.size() == 1000);
}

TEST_CASE("list assign iterator", "[list]") {
	const int table[] = {1, 2, 3, 4, 5};
	constexpr int size = sizeof(table) / sizeof(table[0]);

	DS::list<Type> lst{1};
	lst.assign(table, table + size);
	REQUIRE(lst.size() == size);
	REQUIRE(std::equal(table, table + size, lst.begin()));
}

TEST_CASE("list assign init list", "[list]") {
	std::initializer_list<Type> ilist {1, 2, 3, 4, 5};

	DS::list<Type> lst{1, 2, 3};
	lst.assign(ilist);
	REQUIRE(lst.size() == ilist.size());
	REQUIRE(std::equal(ilist.begin(), ilist.end(), lst.begin()));
}

/*
Insertion */

TEST_CASE("list insert copy constructor", "[list]") {
	DS::list<DefaultCopyOnly> lst;

	DefaultCopyOnly dco;
	lst.insert(lst.end(), dco);
	REQUIRE(lst.size() == 1);
}

TEST_CASE("list insert move constructor", "[list]") {
	DS::list<DefaultMoveOnly> lst;

	lst.insert(lst.end(), DefaultMoveOnly());
	REQUIRE(lst.size() == 1);
}

TEST_CASE("list insert", "[list]") {
	DS::list<Type> lst;

	for (Type i = 0; i < 10; i++) {
		lst.insert(lst.begin(), 42);
	}
	REQUIRE(lst.size() == 10);
	for (auto& val : lst) {
		REQUIRE(val == 42);
	}
}

TEST_CASE("list insert fill", "[list]") {
	DS::list<Type> lst;

	auto it = lst.insert(lst.end(), 10, 42);
	REQUIRE(it == lst.begin());
	REQUIRE(lst.size() == 10);
	for (auto val : lst) {
		REQUIRE(val == 42);
	}
}

TEST_CASE("list insert iterator", "[list]") {
	std::vector<CompoundType> v;
	DS::list<CompoundType> lst;

	v.resize(1000);
	lst.insert(lst.end(), v.begin(), v.end());
	REQUIRE(lst.size() == v.size());
	REQUIRE(std::equal(v.begin(), v.end(), lst.begin()));
	lst.insert(lst.begin(), v.begin(), v.end());
	REQUIRE(lst.size() == 2 * v.size());
}

TEST_CASE("list initializer list", "[list]") {
	DS::list<Type> lst;

	std::initializer_list<Type> init_lst = {1, 2, 3, 4, 5};

	auto it = lst.insert(lst.end(), init_lst);
	REQUIRE(lst.size() == init_lst.size());
	REQUIRE(std::equal(it, lst.end(), init_lst.begin()));
}

TEST_CASE("list emplace", "[list]") {
	/*
	Call copy constructor */
	DS::list<DefaultCopyOnly> l1;
	DefaultCopyOnly dco;
	l1.emplace(l1.end(), dco);
	REQUIRE(l1.size() == 1);

	/*
	Call move constructor */
	DS::list<DefaultMoveOnly> l2;
	DefaultMoveOnly dmo;
	l2.emplace(l2.end(), std::move(dmo));
	REQUIRE(l2.size() == 1);

	/*
	Call specific constructors */
	DS::list<std::vector<int>> l3;
	std::vector<int> v {1, 2, 3, 4, 5};
	l3.emplace(l3.end(), v.begin(), v.end());
	REQUIRE(l3.size() == 1);
	REQUIRE(std::equal(v.begin(), v.end(), l3.front().begin()));
	l3.emplace(l3.end(), 100, 42);
	REQUIRE(l3.back().size() == 100);
}

TEST_CASE("list emplace_back", "[list]") {
	DS::list<std::vector<int>> lst;

	std::vector<std::vector<int>> v;
	for (std::size_t i = 0; i < 42; i++) {
		v.push_back(std::vector<int>(i + 1, 100));
		REQUIRE(v[i].size() == i + 1);
	}
	REQUIRE(v.size() == 42);
	for (const auto& x : v) {
		lst.emplace_back(x);
		REQUIRE(lst.back() == x);
	}
}

TEST_CASE("list push_back constructor test", "[list]") {
	DS::list<DefaultCopyOnly> lst;
	DefaultCopyOnly dco;
	lst.push_back(dco);
	REQUIRE(lst.size() == 1);
	DS::list<DefaultMoveOnly> lst2;
	lst2.push_back(DefaultMoveOnly());
	REQUIRE(lst2.size() == 1);
}

TEST_CASE("list push_back validity test", "[list]") {
	DS::list<Type> lst;

	std::vector<Type> v {1, 2, 3, 4, 5};
	for (auto rit = v.rbegin(); rit != v.rend(); ++rit) {
		lst.push_back(*rit);
	}
	REQUIRE(lst.size() == v.size());
	REQUIRE(lst.back() == v.front());
	REQUIRE(lst.front() == v.back());
	REQUIRE(std::equal(v.begin(), v.end(), lst.rbegin()));
}

TEST_CASE("list push_front constructor test", "[list]") {
	DS::list<DefaultCopyOnly> lst;
	DefaultCopyOnly dco;
	lst.push_front(dco);
	REQUIRE(lst.size() == 1);
	DS::list<DefaultMoveOnly> lst2;
	lst2.push_front(DefaultMoveOnly());
	REQUIRE(lst2.size() == 1);
}

TEST_CASE("list push_front validity test", "[list]") {
	DS::list<Type> lst;

	std::vector<Type> v {1, 2, 3, 4, 5};
	for (auto it = v.begin(); it != v.end(); ++it) {
		lst.push_front(*it);
	}
	REQUIRE(lst.size() == v.size());
	REQUIRE(lst.back() == v.front());
	REQUIRE(lst.front() == v.back());
	REQUIRE(std::equal(v.begin(), v.end(), lst.rbegin()));
}

TEST_CASE("list emplace_front", "[list]") {
	DS::list<std::vector<int>> lst;

	std::vector<std::vector<int>> v;
	for (std::size_t i = 0; i < 42; i++) {
		v.push_back(std::vector<int>(i + 1, 100));
		REQUIRE(v[i].size() == i + 1);
	}
	REQUIRE(v.size() == 42);
	for (const auto& x : v) {
		lst.emplace_front(x);
		REQUIRE(lst.front() == x);
	}
}

TEST_CASE("list resize", "[list]") {
	DS::list<Type> lst {1, 2, 3, 4};
	DS::list<Type> cpy {lst};
	lst.resize(4);
	REQUIRE(lst.size() == cpy.size());
	REQUIRE(std::equal(cpy.begin(), cpy.end(), lst.begin()));
	lst.resize(2);
	REQUIRE(lst.size() == 2);
	lst.resize(10);
	REQUIRE(lst.size() == 10);
	lst.resize(0, 0);
	REQUIRE(lst.empty());
	lst.resize(1, 42);
	lst.resize(50, 42);
	REQUIRE(lst.size() == 50);
	REQUIRE(lst.front() == 42);
}

TEST_CASE("list swap", "[list]") {
	DS::list<Type> lst1 {1, 2, 3, 4};
	DS::list<Type> lst2;

	auto it = lst1.begin();
	auto it2 = lst2.begin();
	lst1.swap(lst2);
	REQUIRE(lst1.empty());
	REQUIRE(it == lst2.begin());
	REQUIRE(it2 == lst1.begin());
	lst2.swap(lst1);
	lst2.swap(lst2);
	REQUIRE(it == lst1.begin());
	REQUIRE(it2 == lst2.begin());
	REQUIRE(lst2.empty());
}

/*
Erasing, Deletion */

TEST_CASE("list erase", "[list]") {
	DS::list<Type> lst(10, 10);

	REQUIRE(isValidList(lst));
	lst.erase(lst.begin());
	REQUIRE(lst.size() == 9);
	REQUIRE(isValidList(lst));
	auto it = lst.begin();
	while (it != lst.end()) {
		lst.erase(it++);
	}
	REQUIRE(isValidList(lst));
	REQUIRE(lst.begin() == lst.end());
}

TEST_CASE("list erase range", "[list]") {
	DS::list<Type> lst(10, 10);

	auto it = lst.begin();
	for (Type i = 0; i < 5; i++) {
		it++;
	}
	lst.erase(it, lst.end());
	REQUIRE(lst.size() == 5);
	REQUIRE(isValidList(lst));

	lst.erase(lst.begin(), lst.end());
	REQUIRE(lst.empty());
	REQUIRE(isValidList(lst));
}

TEST_CASE("list clear", "[list]") {
	DS::list<Type> lst;

	lst.clear();
	REQUIRE(isValidList(lst));
	lst = {1, 2, 3, 4, 5};
	REQUIRE(isValidList(lst));
	lst.clear();
	REQUIRE(isValidList(lst));
	REQUIRE(lst.empty());
	REQUIRE(lst.begin() == lst.end());
}

TEST_CASE("list pop_back", "[list]") {
	DS::list<Type> lst (42, 69);
	lst.push_back(42);
	REQUIRE(lst.size() == 43);
	REQUIRE(lst.back() == 42);
	lst.pop_back();
	REQUIRE(lst.back() == 69);
	int i = 0;
	while (!lst.empty()) {
		lst.pop_back();
		i++;
	}
	REQUIRE(i == 42);
	REQUIRE(lst.empty());
}

TEST_CASE("list pop_front", "[list]") {
	DS::list<Type> lst{1, 2, 3, 4};

	REQUIRE(lst.front() == 1);
	lst.pop_front();
	REQUIRE(lst.front() == 2);
	REQUIRE(lst.size() == 3);
	int i = 0;
	while (!lst.empty()) {
		lst.pop_front();
		i++;
	}
	REQUIRE(i == 3);
}

TEST_CASE("list const iterator conversion", "[list]") {
	DS::list<Type>::iterator x;
	DS::list<Type>::const_iterator y = x;
	DS::list<Type>::const_iterator z(x);
	DS::list<Type>::const_iterator a;
	a = x;
	REQUIRE(y == x);
	REQUIRE(x == y);
	REQUIRE(z == x);
	REQUIRE(a == x);
	REQUIRE(!(x != x));
}
