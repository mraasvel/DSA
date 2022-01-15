#include "algorithms/algorithms.hpp"
#include <catch2/catch.hpp>
#include <vector>

TEST_CASE("Insertion Sort", "[sort]") {
	std::vector<int> v {5, 4, 3, 2, 1};
	DSA::insertionSort(v.begin(), v.end());
	REQUIRE(std::is_sorted(v.begin(), v.end()));
}
