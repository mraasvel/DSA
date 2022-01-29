#include "algorithms/maximum_subarray.hpp"
#include <vector>
#include <catch2/catch.hpp>
#include <cstdlib>
#include <ctime>
#include <random>

int randomRangeMersenne(int min, int max) {
	static std::mt19937 mersenne(static_cast<std::mt19937::result_type>(std::time(nullptr)));
	std::uniform_int_distribution<> die(min, max);
	return die(mersenne);
}

std::vector<int> randomVector(size_t n, int max = std::numeric_limits<int>::max()) {
	std::vector<int> v;
	while (n-- > 0) {
		int x = randomRangeMersenne(1, max);
		x = randomRangeMersenne(1, max) % 2 == 0 ? x : -x;
		v.push_back(x);
	}
	return v;
}

TEST_CASE("maximum subarray", "[algorithm]") {
	std::vector<int> v {13, -3, -25, 20, -3, -16, -23, 18, 20, -7, 12, -5, -22, 15, -4, 7};
	int result = DSA::maximumSubarray(v.begin(), v.end());
	REQUIRE(result == 43);
}

TEST_CASE("maximum subarray BF", "[algorithm]") {
	std::vector<int> v {13, -3, -25, 20, -3, -16, -23, 18, 20, -7, 12, -5, -22, 15, -4, 7};
	int result = DSA::maximumSubarrayBF(v.begin(), v.end());
	REQUIRE(result == 43);
}

TEST_CASE("maximum subarray Linear", "[algorithm]") {
	std::vector<int> v {13, -3, -25, 20, -3, -16, -23, 18, 20, -7, 12, -5, -22, 15, -4, 7};
	int result = DSA::maximumSubarrayLinear(v.begin(), v.end());
	REQUIRE(result == 43);
}

TEST_CASE("maximum subarray validity" "[algorithm]") {
	for (int i = 0; i < 100; ++i) {
		std::vector<int> v { randomVector(100, 1000) };
		auto result = DSA::maximumSubarrayBF(v.begin(), v.end());
		REQUIRE(result == DSA::maximumSubarray(v.begin(), v.end()));
		REQUIRE(result == DSA::maximumSubarrayLinear(v.begin(), v.end()));
	}
}
