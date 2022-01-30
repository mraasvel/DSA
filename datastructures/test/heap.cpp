#include "heap/heap.hpp"
#include <limits>
#include <catch2/catch.hpp>
#include "algorithm"

template <typename HeapType>
static bool validHeap(const HeapType& h) {
	return std::is_heap(h.begin(), h.end(), h.value_comp());
}

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
	REQUIRE(validHeap(h));
	while (!h.empty()) {
		int x = h.top();
		REQUIRE(x <= prev);
		prev = x;
		h.pop();
	}
	REQUIRE(validHeap(h));
}

static void testMinHeap(std::size_t n) {
	auto h {randomHeap<std::greater<int>>(n)};
	int prev = std::numeric_limits<int>::min();
	REQUIRE(validHeap(h));
	while (!h.empty()) {
		int x = h.top();
		REQUIRE(x >= prev);
		prev = x;
		h.pop();
	}
	REQUIRE(validHeap(h));
}

TEST_CASE("heap min", "[heap]") {
	testMinHeap(1000);
}

TEST_CASE("heap unit", "[heap]") {
	constexpr int ITERATIONS = 1000;
	for (int i = 0; i < ITERATIONS; ++i) {
		testMinHeap(50);
	}
}
