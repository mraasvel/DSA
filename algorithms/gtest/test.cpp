#include "algorithms/algorithms.hpp"
#include <gtest/gtest.h>
#include <algorithm>
#include <vector>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
	std::vector<int> v {5, 4, 3, 2, 1};
	DSA::insertionSort(v.begin(), v.end());
	EXPECT_EQ(std::is_sorted(v.begin(), v.end()), true);
}
