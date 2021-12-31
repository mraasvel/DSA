#include "redblack/redblack.hpp"
#include "valid_rb_test.hpp"
#include "catch.hpp"

/* Constructors */

TEST_CASE("red-black default constructor", "[red-black]") {

	RedBlackTree<int> s;

	REQUIRE(s.empty() == true);
	REQUIRE(s.size() == 0);
	REQUIRE(Test::validRB(s) == true);
}

TEST_CASE("red-black range insertion", "[red-black]") {
	const int table[] = {0, 1, 2, 3, 4};
	const int size = sizeof(table) / sizeof(int);

	RedBlackTree<int> s;

	for (int i = 0; i < size; ++i) {
		s.insert(table[i]);
	}

	RedBlackTree<int>::const_iterator it = s.begin();
	RedBlackTree<int>::const_iterator ite = s.end();

	REQUIRE(s.size() == size);

	int i = 0;
	while (it != ite) {
		REQUIRE(i == *it);
		++it;
		++i;
	}
	REQUIRE(Test::validRB(s));
}

TEST_CASE("RedBlackTree random insertion", "[RedBlackTree]") {
	std::vector<int> v;
	srand(time(0));
	for (int i = 0; i < 100; ++i) {
		v.push_back(rand());
	}


	RedBlackTree<int> s;

	for (auto it = v.begin(); it != v.end(); ++it) {
		s.insert(*it);
	}

	std::sort(v.begin(), v.end());

	auto it2 = v.begin();
	for (auto it = s.begin(); it != s.end(); ++it, ++it2) {
		REQUIRE(*it == *it2);
	}
	REQUIRE(Test::validRB(s));
}

TEST_CASE("RedBlackTree copy constructor", "[RedBlackTree]") {
	RedBlackTree<int> s;

	for (int i = 0; i < 100; ++i) {
		s.insert(rand());
	}

	RedBlackTree<int> s2(s);

	RedBlackTree<int>::iterator it = s.begin();
	RedBlackTree<int>::iterator it2 = s2.begin();
	REQUIRE(s == s2);
	REQUIRE(Test::validRB(s));
	REQUIRE(Test::validRB(s2));
}

// /* Assignment */

TEST_CASE("RedBlackTree copy assignment", "[RedBlackTree]") {
	RedBlackTree<int> s;

	for (int i = 0; i < 100; ++i) {
		s.insert(rand());
	}

	RedBlackTree<int> s2;
	s2 = s;
	REQUIRE(s == s2);
	s.clear();
	s2 = s;
	REQUIRE(s == s2);
	REQUIRE(Test::validRB(s));
	REQUIRE(Test::validRB(s2));
}

// /* Iterators */

TEST_CASE("RedBlackTree iterators", "[RedBlackTree]") {
	RedBlackTree<int> s;

	REQUIRE(s.begin() == s.end());
	for (int i = -100; i < 100; ++i) {
		s.insert(i);
	}

	REQUIRE(s.size() == 200);

	auto it = s.begin();
	for (int i = -100; i < 100; ++i) {
		REQUIRE(i == *it);
		++it;
	}
	REQUIRE(Test::validRB(s));
}

TEST_CASE("RedBlackTree const iterators", "[RedBlackTree]") {
	std::vector<int> v;

	for (int i = -100; i < 100; ++i) {
		v.push_back(i);
	}

	RedBlackTree<int> s0;
	for (auto it = v.begin(); it != v.end(); ++it) {
		s0.insert(*it);
	}

	const RedBlackTree<int> s(s0);

	RedBlackTree<int>::const_iterator it = s.begin();
	while (it != s.end()) {
		++it;
	}
	REQUIRE(it == s.end());
	it = s.begin();
	REQUIRE(s.size() == 200);

	for (int i = -100; i < 100; ++i) {
		REQUIRE(i == *it);
		++it;
	}
	REQUIRE(Test::validRB(s));
}

TEST_CASE("RedBlackTree iterator to const iterator conversion", "[RedBlackTree]") {
	RedBlackTree<int> s;

	s.insert(42);
	RedBlackTree<int>::iterator it = s.begin();
	RedBlackTree<int>::iterator ite = s.end();
	RedBlackTree<int>::const_iterator const_it = it;
	RedBlackTree<int>::const_iterator const_ite = ite;

	REQUIRE(it == const_it);
	REQUIRE(const_it == it);
	REQUIRE(ite == const_ite);
	REQUIRE(const_ite == ite);
}

TEST_CASE("RedBlackTree iterator other", "[RedBlackTree]") {
	RedBlackTree<int> s;
	RedBlackTree<int> s2;

	REQUIRE(s.begin() != s2.begin());
	REQUIRE(s.begin() == s.end());
	REQUIRE(s.end() != s2.end());
}

TEST_CASE("RedBlackTree iterator end testing", "[RedBlackTree]") {
	RedBlackTree<int> s;

	s.insert(42);

	auto it = s.end();
	--it;
	REQUIRE(*it == 42);
}

TEST_CASE("dereferencing iterator", "[RedBlackTree]") {
	struct Compound {
		Compound(int x = 0, int y = 0)
		: x(x), y(y) {}
		bool operator<(const Compound& rhs) const {
			return x < rhs.x || (x == rhs.x && y < rhs.y);
		}
		int x, y;
	};
	RedBlackTree<Compound> s;
	s.insert(Compound(2, 5));
	const RedBlackTree<Compound>::iterator it = s.begin();
	it->x = 5;
	REQUIRE(s.begin()->x == 5);
}

// /* Capacity */

TEST_CASE("RedBlackTree size", "[RedBlackTree]") {
	RedBlackTree<int> s;

	REQUIRE(s.size() == 0);
	s.insert(42);
	REQUIRE(s.size() == 1);
	for (int i = 0; i < 100; ++i) {
		s.insert(i);
	}

	REQUIRE(s.size() == 100);
	REQUIRE(Test::validRB(s));
	s.clear();
	REQUIRE(s.size() == 0);
	s.clear();
	REQUIRE(Test::validRB(s));
}

// /* Modifiers */

TEST_CASE("RedBlackTree insert single element", "[RedBlackTree]") {
	RedBlackTree<int> s;

	const int MAX_SIZE = 100000;

	s.insert(42);

	REQUIRE(*s.begin() == 42);
	REQUIRE(s.size() == 1);

	for (int i = 0; i < MAX_SIZE; ++i) {
		s.insert(i);
	}
	REQUIRE(s.size() == MAX_SIZE);
	REQUIRE(Test::validRB(s));
}

TEST_CASE("RedBlackTree erase position", "[RedBlackTree]") {
	RedBlackTree<int> s;

	s.insert(42);
	s.erase(s.begin());
	REQUIRE(s.size() == 0);

	for (int i = 0; i < 50; ++i) {
		s.insert(i);
	}

	auto it = s.begin();
	for (int i = 0; i < 25; ++i) {
		++it;
	}

	s.erase(it++);
	REQUIRE(Test::validRB(s));
}

TEST_CASE("Red-Black erase", "[red-black]") {
	RedBlackTree<int> m;

	m.insert(1);
	m.insert(2);
	REQUIRE(Test::validRB(m));
	m.erase(1);
	REQUIRE(Test::validRB(m));
}

TEST_CASE("Red-Black erase case 0", "[red-black]") {
	RedBlackTree<int> m;

	for (int i = 0; i < 6; ++i) {
		m.insert(i);
	}

	m.erase(4);
	REQUIRE(m.size() == 5);

	for (int i = 0; i < 10000; ++i) {
		m.insert(i);
	}

	REQUIRE(m.size() == 10000);

	auto it = m.begin();
	while (it != m.end()) {
		m.erase(it++);
	}
	REQUIRE(m.size() == 0);
	REQUIRE(Test::validRB(m));
	m.insert(42);
	for (auto it = m.begin(); it != m.end(); ++it) {
		REQUIRE(*it == 42);
	}
	REQUIRE(m.size() == 1);
}

TEST_CASE("Red-Black erase case 1", "[red-black]") {
	RedBlackTree<int> m;

	for (int i = 0; i < 10; ++i) {
		m.insert(i);
	}

	m.erase(4);

	for (int i = 0; i < 10000; ++i) {
		m.insert(i);
	}

	REQUIRE(Test::validRB(m));

	for (auto it = m.begin(); it != m.end();) {
		m.erase(it++);
	}

	m.insert(42);

	REQUIRE(m.size() == 1);
}

TEST_CASE("Red-Black erase case 2", "[red-black]") {
	RedBlackTree<int> m;

	for (int i = 0; i < 8; ++i) {
		m.insert(i);
	}

	m.erase(1);
	REQUIRE(Test::validRB(m));
}

TEST_CASE("Red-Black erase case 3", "[red-black]") {
	RedBlackTree<int> m;

	for (int i = 0; i < 16; ++i) {
		m.insert(i);
	}

	m.erase(2);
	m.erase(1);
	m.erase(5);
	m.insert(5);
	m.erase(6);
	REQUIRE(Test::validRB(m));
	m.erase(0);
	REQUIRE(Test::validRB(m));
}

TEST_CASE("Red-Black erase case 4", "[red-black]") {
	RedBlackTree<int> m;

	for (int i = 0; i < 6; ++i) {
		m.insert(i);
	}

	m.erase(3);
	REQUIRE(Test::validRB(m));
}


TEST_CASE("RedBlackTree swap", "[RedBlackTree]") {
	RedBlackTree<int> s;
	RedBlackTree<int> s2;

	s.insert(42);
	s2.insert(420);

	REQUIRE(*s.begin() == 42);
	REQUIRE(*s2.begin() == 420);

	RedBlackTree<int>::iterator it = s.begin();
	RedBlackTree<int>::iterator ite = s.end();
	(void)ite;

	s.swap(s2);
	REQUIRE(*s.begin() == 420);
	REQUIRE(*s2.begin() == 42);
	REQUIRE(it == s2.begin());

	++it;
	REQUIRE(it == s2.end());
	it--;
	REQUIRE(it == s2.begin());
}

TEST_CASE("RedBlackTree clear", "[RedBlackTree]") {
	RedBlackTree<int> s;

	s.clear();
	s.clear();

	for (int i = 0; i < 100; ++i) {
		s.insert(i);
	}

	REQUIRE(s.size() == 100);

	int i = 0;
	for (RedBlackTree<int>::iterator it = s.begin(); it != s.end(); it++, i++) {
		REQUIRE(i == *it);
	}
	REQUIRE(i == 100);

	RedBlackTree<int>::iterator ite = s.end();

	s.clear();
	REQUIRE(s.begin() == s.end());

	s.insert(42);
	--ite;
	REQUIRE(*ite == 42);
}

// /* Operations */

TEST_CASE("RedBlackTree find", "[RedBlackTree]") {
	RedBlackTree<int> s;

	for (int i = 0; i < 100; ++i) {
		s.insert(i);
	}

	RedBlackTree<int>::iterator it = s.find(42);
	RedBlackTree<int>::const_iterator cit = s.find(42);

	REQUIRE(it != s.end());
	REQUIRE(cit != s.end());
	REQUIRE(*it == 42);
	REQUIRE(*cit == 42);

	it = s.find(420);
	cit = s.find(420);

	REQUIRE(it == s.end());
	REQUIRE(cit == s.end());
}

TEST_CASE("RedBlackTree count", "[RedBlackTree]") {
	RedBlackTree<int> s;

	const int SIZE = 20;
	const int TEST_SIZE = SIZE + 20;

	REQUIRE(s.count(42) == 0);

	for (int i = 0; i < SIZE; ++i) {
		s.insert(i);
	}


	for (int i = 0; i < TEST_SIZE; ++i) {
		if (i < SIZE) {
			REQUIRE(s.count(i) == 1);
		} else {
			REQUIRE(s.count(i) == 0);
		}
	}
}

TEST_CASE("RedBlackTree lower bound", "[RedBlackTree]") {
	RedBlackTree<int> s;

	for (int i = 1; i < 10; ++i) {
		s.insert(i * 10);
	} // 10 20 30 40 50 60 70 80 90

	RedBlackTree<int>::iterator it = s.lower_bound(30);
	REQUIRE(*it == 30);
	it = s.lower_bound(35);
	REQUIRE(*it == 40);
	it = s.lower_bound(95);
	REQUIRE(it == s.end());
	it = s.lower_bound(0);
	REQUIRE(*it == 10);
	REQUIRE(it == s.begin());
}

TEST_CASE("RedBlackTree upper bound", "[RedBlackTree]") {
	RedBlackTree<int> s;

	for (int i = 1; i < 10; ++i) {
		s.insert(i * 10);
	} // 10 20 30 40 50 60 70 80 90

	RedBlackTree<int>::iterator it = s.upper_bound(10);
	REQUIRE(*it == 20);
	it = s.upper_bound(90);
	REQUIRE(it == s.end());
	it = s.upper_bound(10);
	REQUIRE(*it == 20);
	it = s.upper_bound(5);
	REQUIRE(*it == 10);
	REQUIRE(it == s.begin());
}

TEST_CASE("RedBlackTree equal range", "[RedBlackTree]") {
	RedBlackTree<int> s;

	for (int i = 1; i < 10; ++i) {
		s.insert(i * 10);
	}

	std::pair<RedBlackTree<int>::iterator, RedBlackTree<int>::iterator> p;

	p = s.equal_range(10);

	REQUIRE(*(p.first) == 10);
	REQUIRE(*(p.second) == 20);
	REQUIRE(p.first == s.lower_bound(10));
	REQUIRE(p.second == s.upper_bound(10));

	p = s.equal_range(15);
	REQUIRE(*(p.first) == 20);
	REQUIRE(*(p.first) == *(p.second));
	REQUIRE(p.first == p.second);

	s.erase(p.first);
	p = s.equal_range(10);
	REQUIRE(*(p.first) == 10);
	REQUIRE(*(p.second) == 30);
}

// /* Comparisons */

TEST_CASE("RedBlackTree comparison operators", "[RedBlackTree]") {

	std::vector<int> v_alice{1, 2, 3};
	std::vector<int> v_bob{7, 8, 9, 10};
	std::vector<int> v_eve{1, 2, 3};

	RedBlackTree<int> alice;
	RedBlackTree<int> bob;
	RedBlackTree<int> eve;
	for (auto it = v_alice.begin(); it != v_alice.end(); ++it) {
		alice.insert(*it);
	}
	for (auto it = v_bob.begin(); it != v_bob.end(); ++it) {
		bob.insert(*it);
	}
	for (auto it = v_eve.begin(); it != v_eve.end(); ++it) {
		eve.insert(*it);
	}

	REQUIRE(!(alice == bob));
	REQUIRE(alice != bob);
	REQUIRE(alice < bob);
	REQUIRE(alice <= bob);
	REQUIRE(!(alice > bob));
	REQUIRE(!(alice >= bob));

	REQUIRE(alice == eve);
	REQUIRE(!(alice != eve));
	REQUIRE(!(alice < eve));
	REQUIRE(alice <= eve);
	REQUIRE(!(alice > eve));
	REQUIRE(alice >= eve);
}

// /* const tests */

TEST_CASE("RedBlackTree const type", "[RedBlackTree]") {
	std::vector<int> v{0, 1, 2, 3, 4, 5};
	RedBlackTree<int> m;
	for (auto it = v.begin(); it != v.end(); ++it) {
		m.insert(*it);
	}
	const RedBlackTree<int> s(m);

	REQUIRE(s.count(0) == 1);

	RedBlackTree<int>::const_iterator it = s.find(3);
	(void)it;
	REQUIRE(s.count(3) == 1);

	REQUIRE(s.lower_bound(3) == s.find(3));
	REQUIRE(s.upper_bound(3) == s.find(4));
	REQUIRE(s.equal_range(3) == std::make_pair(s.find(3), s.find(4)));
	std::less<int> l = s.value_comp();
	l = s.value_comp();
	REQUIRE(!s.empty());
	REQUIRE(s.size() == v.size());
	s.max_size();
	s.begin();
	s.end();
}
