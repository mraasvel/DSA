#pragma once

#include "permutations.hpp"
#include <iostream> //REMOVE
#include <cassert> // REMOVE

namespace Testing {

template <typename Container, std::size_t num_iterators = 1>
class IteratorPermutations {
private:
	using value_type = typename Container::iterator;
	using table_type = std::array<value_type, num_iterators>;
	using permutations_type = util::ReplacementPermutations<int>;
	using permutation_type = typename permutations_type::permutation_type;
public:
	using iterator = typename table_type::iterator;
	using const_iterator = typename table_type::const_iterator;
public:
	IteratorPermutations(const Container& c)
	: original(c), permutations(num_iterators, 0, c.size()) {
		generatePermutation();
	}

	iterator begin() {
		return table.begin();
	}

	const_iterator begin() const {
		return table.cbegin();
	}

	iterator end() {
		return table.end();
	}

	const_iterator end() const {
		return table.cend();
	}

	Container& getContainer() {
		return c;
	}

	const Container& getContainer() const {
		return c;
	}

	value_type operator[](std::size_t index) const {
		return table[index];
	}

	IteratorPermutations& operator++() {
		permutations.nextPermutation();
		generatePermutation();
		return *this;
	}

	bool isLast() const {
		return permutations.isEnd();
	}

private:
	void generatePermutation() {
		c = original;
		auto perm = permutations.getPermutation();
		for (size_t i = 0; i < table.size(); ++i) {
			table[i] = c.begin();
			std::advance(table[i], perm[i]);
		}
	}

private:
	table_type table;
	Container c;
	Container original;
	permutations_type permutations;
};

}
