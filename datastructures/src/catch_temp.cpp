#include "list/list.hpp"
#include "Testing/testclass.hpp"
#include "Testing/list_invariant.hpp"
#include "Testing/iterator_permutations.hpp"
#include <list>
#include <catch2/catch.hpp>

using namespace Testing;
typedef Testing::TestClass<int> TC;

// #define DS std
template <typename Permutations>
static void printPermutation(Permutations& permutations) {
	auto& c = permutations.getContainer();
	std::cout << "Permutation:";
	for (const auto& it : permutations) {
		std::cout << ' ';
		if (it == c.end()) {
			std::cout << "E";
		} else {
			std::cout << *it;
		}
	}
	std::cout << std::endl;
}

template <typename Permutations>
static void printPermutations(Permutations& permutations) {
	while (!permutations.isLast()) {
		printPermutation(permutations);
		++permutations;
	}
}

TEST_CASE("Temporary Testing", "[tmp]") {
	std::list<int> lst {1, 2, 3};
	IteratorPermutations<std::list<int>, 2> permutation {lst};
	// printPermutations(permutation);
}
