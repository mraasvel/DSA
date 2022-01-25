#pragma once

#include <algorithms/sfinae.hpp>
#include <limits>

namespace DSA {

	namespace Detail {
	
	template <typename InputIterator>
	int maximumSum(InputIterator first, InputIterator last) {
		int max_sum = std::numeric_limits<int>::min();
		int sum = 0;
		while (first != last) {
			sum += *first;
			max_sum = std::max(max_sum, sum);
			++first;
		}
		return max_sum;
	}

	template <typename BidirectionalIterator>
	int maximumCrossSubarray(BidirectionalIterator first, BidirectionalIterator last,
							BidirectionalIterator mid) {
		using ReverseIterator = std::reverse_iterator<BidirectionalIterator>;
		int sumleft = maximumSum(ReverseIterator{mid}, ReverseIterator{first});
		int sumright = maximumSum(mid, last);
		return sumleft + sumright;
	}

	}

/*
Divide and conquer: O(n log n)

T(1) = O(1)
T(n) = 2 * T(n / 2) + O(n)
*/

template <typename BidirectionalIterator,
		DSA::RequireBidirectionalIterator<BidirectionalIterator> = true>
int maximumSubarray(BidirectionalIterator first, BidirectionalIterator last) {
	auto dist = std::distance(first, last);
	if (dist == 0) {
		return 0;
	} else if (dist == 1) {
		return *first;
	}
	auto mid = std::next(first, dist / 2);
	int left = maximumSubarray(first, mid);
	int right = maximumSubarray(mid, last);
	int cross = Detail::maximumCrossSubarray(first, last, mid);
	return std::max(std::max(left, right), cross);
}

}
