#include "algorithms/algorithms.hpp"
#include <catch2/catch.hpp>
#include <vector>
#include <ctime>
#include <random>

using ContainerType = std::vector<int>;
using IteratorType = ContainerType::iterator;

static int randomIntRange(int min, int max) {
	static std::mt19937 mersenne(static_cast<std::mt19937::result_type>(std::time(nullptr)));
	std::uniform_int_distribution<> die(min, max);
	return die(mersenne);
}

static ContainerType randomContainer(std::size_t n) {
	ContainerType x;
	x.reserve(n);
	while (n-- > 0) {
		x.push_back(randomIntRange(0, 10000));
	}
	return x;
}

template <typename Sorter>
static void testSort(Sorter sorter) {
	static std::vector<ContainerType> testcases {
		{},
		{1},
		{2, 1},
		{1, 2, 3, 4},
		{4, 3, 2, 1},
		randomContainer(100),
		randomContainer(100),
	};

	for (std::size_t i = 0; i < testcases.size(); ++i) {
		auto testcase = testcases[i];
		SECTION(std::to_string(i)) {
			auto copy {testcase};
			std::sort(copy.begin(), copy.end());
			sorter(testcase.begin(), testcase.end());
			REQUIRE(copy == testcase);
			REQUIRE(std::is_sorted(testcase.begin(), testcase.end()));
		}
	}
}

TEST_CASE("Insertion Sort", "[sort]") {
	testSort(&DSA::insertionSort<IteratorType>);
}

TEST_CASE("Recursive Insertion Sort", "[sort]") {
	testSort(&DSA::recursiveInsertionSort<IteratorType>);
}

TEST_CASE("Selection Sort", "[sort]") {
	testSort(&DSA::selectionSort<IteratorType>);
}

TEST_CASE("Merge Sort", "[sort]") {
	testSort(&DSA::mergeSort<IteratorType>);
}

TEST_CASE("Merge Insertion Sort", "[sort]") {
	testSort(&DSA::mergeInsertionSort<IteratorType>);
}

TEST_CASE("Bubble Sort", "[sort]") {
	testSort(&DSA::bubbleSort<IteratorType>);
}

TEST_CASE("Heap Sort", "[sort]") {
	testSort(&DSA::heapSort<IteratorType>);
}
