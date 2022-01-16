#pragma once

#include "sfinae.hpp"
#include <functional>



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
	for (auto x = std::next(first); x != last; ++x) {
		auto key = *x;
		auto y = std::prev(x);
		while (y >= first && comp(key, *y)) {
			*std::next(y) = *y;
			--y;
		}
		*std::next(y) = key;
	}
}

template <typename RandomAccessIt>
void insertionSort(RandomAccessIt first, RandomAccessIt last) {
	if (first == last) {
		return;
	}
	insertionSort(first, last, std::less<decltype(*first)>());
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
	if (first == last) {
		return;
	}
	selectionSort(first, last, std::less<decltype(*first)>());
}

}
