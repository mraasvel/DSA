#pragma once

#include "sfinae.hpp"
#include <functional>
#include <vector>
#include <iostream> // REMOVE

namespace DSA {

/*
Insertion Sort */

/*
Efficient for sorting a list with a small number of elements.
Sorted subset that starts out as empty, and an unsorted subset that starts out as full.
Take the top card from the unsorted subset, and insert it into the sorted subset. */

/*
Runtime Analysis:
Outer loop: iterate once over (n - 1) elements.
Inner loop: iterator over (n - x) elements until we find the sorted position.
(n - x) / 2 elements (work)
Number of elements iterated in total:
1 + 2 + ... + (n - 1)
n(n + 1) / 2 -> (n - 1)(n) / 2
So the average elements for the inner loop is n / 4
So we get n * n / 4 = 1/4 * n^2 = O(n^2)
Total elements = 0.5 (n^2 - n) = n^2 / 2 - n / 2 = n^2 - n = O(n^2) */

/*
Loop invariant (true before and after each iteration):
	elements [first, x) are in sorted order.
	elements [x, last) are in any order.

Initialization:
	elements [first, x) are in sorted order because this is only one element at this point.
Maintenance:
	First every element that is not in order is moved one space to the right.
	Then the key is inserted into it's correct position, making the range [first, x) sorted for the next iteration.
Termination:
	The loop temrinates when we reach the end of the array.
	Using the loop invariant: [first, last), we can conclude that the elements are in sorted order.
*/

template <typename RandomAccessIt, typename Compare,
	RequireRandomAccessIterator<RandomAccessIt> = true>
void insertionSort(RandomAccessIt first, RandomAccessIt last, Compare comp) {
	if (first == last) {
		return;
	}
	for (auto x = std::next(first); x != last; ++x) {
		auto key = *x;
		auto y = x;
		while (y > first && comp(key, *std::prev(y))) {
			*y = *std::prev(y);
			--y;
		}
		*y = key;
	}
}

template <typename RandomAccessIt>
void insertionSort(RandomAccessIt first, RandomAccessIt last) {
	insertionSort(first, last, std::less<decltype(*first)>());
}

template <typename RandomAccessIt, typename Compare,
	RequireRandomAccessIterator<RandomAccessIt> = true>
void recursiveInsertionSort(RandomAccessIt first, RandomAccessIt last, Compare comp) {
	if (std::distance(first, last) <= 1) {
		return;
	}
	auto pos = std::prev(last);
	auto key = *pos;
	recursiveInsertionSort(first, pos, comp);
	while (pos > first  && comp(key, *std::prev(pos))) {
		*pos = *std::prev(pos);
		--pos;
	}
	*pos = key;
}

template <typename RandomAccessIt>
void recursiveInsertionSort(RandomAccessIt first, RandomAccessIt last) {
	recursiveInsertionSort(first, last, std::less<decltype(*first)>());
}

/*
Loop invariant: the range [first, i) is sorted and [i, last) contains no elements smaller than the elements in range [first, i)
Work:
Work      Times
c1        (n - 1)
c2        (n - 1)
c3(n - i) (n - 1)

Best case: same as worst case since the same operations are always executed
Runtime complexity: Theta of n squared or O(n^2) */
template <typename RandomAccessIt, typename Compare,
	RequireRandomAccessIterator<RandomAccessIt> = true>
void selectionSort(RandomAccessIt first, RandomAccessIt last, Compare comp) {
	if (first == last) {
		return;
	}
	for (auto i = first; i != last - 1; ++i) {
		auto min_element = i;
		for (auto j = i + 1; j != last; ++j) {
			if (*j < *min_element) {
				min_element = j;
			}
		}
		std::swap(*i, *min_element);
	}
}

template <typename RandomAccessIt>
void selectionSort(RandomAccessIt first, RandomAccessIt last) {
	selectionSort(first, last, std::less<decltype(*first)>());
}

	namespace Detail {

	template <typename RandomAccessIt, typename Compare, typename Container>
	void merge(RandomAccessIt first, RandomAccessIt midpoint,
				RandomAccessIt last, Compare comp,
				Container& out) {
		std::copy(first, last, out.begin());
		auto left = out.begin();
		auto right = out.begin() + std::distance(first, midpoint);
		auto left_end = right;
		auto right_end = right + std::distance(midpoint, last);
		while (first != last) {
			if (right == right_end || (left != left_end && comp(*left, *right))) {
				*first++ = *left++;
			} else {
				*first++ = *right++;
			}
		}
	}

	/*
	Out is a container that has at least enough space for [first, last) */
	template <typename RandomAccessIt, typename Compare, typename Container>
	void mergeSort(RandomAccessIt first, RandomAccessIt last, Compare comp, Container& out) {
		if (std::distance(first, last) <= 1) {
			return;
		}
		RandomAccessIt midpoint = first + (last - first) / 2;
		mergeSort(first, midpoint, comp, out);
		mergeSort(midpoint, last, comp, out);
		merge(first, midpoint, last, comp, out);
	}

	template <typename RandomAccessIt, typename Compare, typename Container>
	void mergeInsertionSort(RandomAccessIt first, RandomAccessIt last, Compare comp, Container& out) {
		constexpr int k = 43;
		if (std::distance(first, last) <= k) {
			return insertionSort(first, last, comp);
		}
		RandomAccessIt midpoint = first + (last - first) / 2;
		mergeSort(first, midpoint, comp, out);
		mergeSort(midpoint, last, comp, out);
		merge(first, midpoint, last, comp, out);
	}

	}

/*
Log(n) calls, where n is distance(first, last)
Merge = O(n) where n is distance(first, last)
1 * n + 2 * n/2 + 4 * n/4 + ... + n * 1 = n * numcalls = n * log(n)
Runtime: O(n log n) */

template <typename RandomAccessIt, typename Compare,
	RequireRandomAccessIterator<RandomAccessIt> = true>
void mergeSort(RandomAccessIt first, RandomAccessIt last, Compare comp) {
	using TempContainerType = std::vector<typename std::remove_reference<decltype(*first)>::type>;
	TempContainerType out (std::distance(first, last));
	Detail::mergeSort(first, last, comp, out);
}

template <typename RandomAccessIt>
void mergeSort(RandomAccessIt first, RandomAccessIt last) {
	mergeSort(first, last, std::less<decltype(*first)>());
}

template <typename RandomAccessIt, typename Compare,
	RequireRandomAccessIterator<RandomAccessIt> = true>
void mergeInsertionSort(RandomAccessIt first, RandomAccessIt last, Compare comp) {
	using TempContainerType = std::vector<typename std::remove_reference<decltype(*first)>::type>;
	TempContainerType out (std::distance(first, last));
	Detail::mergeInsertionSort(first, last, comp, out);
}

template <typename RandomAccessIt>
void mergeInsertionSort(RandomAccessIt first, RandomAccessIt last) {
	mergeInsertionSort(first, last, std::less<decltype(*first)>());
}


}
