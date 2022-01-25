#include "algorithms/maximum_subarray.hpp"
#include <vector>
#include <catch2/catch.hpp>

TEST_CASE("maximum subarray", "[algorithm]") {
	std::vector<int> v {13, -3, -25, 20, -3, -16, -23, 18, 20, -7, 12, -5, -22, 15, -4, 7};
	int result = DSA::maximumSubarray(v.begin(), v.end());
	REQUIRE(result == 43);
}
