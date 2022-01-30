#include "heap/heap.hpp"
#include <limits>
#include <catch2/catch.hpp>

template <typename Compare = std::less<int>>
DSA::Heap<int, std::vector<int>, Compare> randomHeap(size_t n, int limit = std::numeric_limits<int>::max()) {
	DSA::Heap<int, std::vector<int>, Compare> h;
	for (size_t i = 0; i < n; ++i) {
		h.push(rand() % limit);
	}
	return h;
}

TEST_CASE("heap max", "[heap]") {
	DSA::Heap<int> h {randomHeap(1000)};
	int prev = std::numeric_limits<int>::max();
	while (!h.empty()) {
		int x = h.top();
		REQUIRE(x <= prev);
		prev = x;
		h.pop();
	}
}

TEST_CASE("heap min", "[heap]") {
	auto h {randomHeap<std::greater<int>>(1000)};
	int prev = std::numeric_limits<int>::min();
	while (!h.empty()) {
		int x = h.top();
		REQUIRE(x >= prev);
		prev = x;
		h.pop();
	}
}
