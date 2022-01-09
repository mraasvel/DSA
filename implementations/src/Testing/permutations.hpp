#ifndef PERMUTATIONS_HPP
#define PERMUTATIONS_HPP

#include <vector>
#include <algorithm>

namespace util {

template <typename T, typename PermutationGenerator>
class PermutationIterator {
public:
	using value_type = typename PermutationGenerator::permutation_type;
public:
	PermutationIterator(PermutationGenerator* x)
	: generator(x) {}

	const value_type& operator*() {
		return generator->getPermutation();
	}

	PermutationIterator& operator++() {
		generator->nextPermutation();
		return *this;
	}

	/*
	Hacky comparison for range-based for loop syntax */
	friend bool operator==(const PermutationIterator& lhs, const PermutationIterator& rhs) {
		return lhs.generator->isEnd() && rhs.generator->isEnd();
	}

	friend bool operator!=(const PermutationIterator& lhs, const PermutationIterator& rhs) {
		return !(lhs == rhs);
	}

private:
	PermutationGenerator* generator;
};

/*
Generate list of n permutations of [start, end]
With/without replacement? https://web.ma.utexas.edu/users/parker/sampling/repl.htm
Complexity: O(distance(first, last)^n) */

/*
Iterator like object, that generates the next permutation */
template <typename T>
class ReplacementPermutations {
public:
	using value_type = T;
	using permutation_type = std::vector<T>;
	using iterator = PermutationIterator<T, ReplacementPermutations>;
public:
	ReplacementPermutations(std::size_t n, const T& first, const T& last)
	: permutation(n, first), first(first), last(last), done(false) {}

	iterator begin() {
		return iterator(this);
	}

	iterator end() {
		return iterator(this);
	}

	const permutation_type& getPermutation() const {
		return permutation;
	}

	void nextPermutation() {
		for (auto rit = permutation.rbegin(); rit != permutation.rend(); ++rit) {
			if (*rit != last) {
				++(*rit);
				return;
			} else {
				*rit = first;
			}
		}
		done = true;
	}

	bool isEnd() const {
		return done;
	}

	void reset() {
		done = false;
	}

private:
	permutation_type permutation;
	T first;
	T last;
	bool done;
};

template <typename T>
class Permutations {
public:
	using value_type = T;
	using permutation_type = std::vector<T>;
	using iterator = PermutationIterator<T, Permutations>;
public:
	Permutations(T first, const T& last)
	: done(false) {
		while (first != last) {
			permutation.emplace_back(first);
			++first;
		}
		permutation.emplace_back(last);
	}

	iterator begin() {
		return iterator(this);
	}

	iterator end() {
		return iterator(this);
	}

	const permutation_type& getPermutation() const {
		return permutation;
	}

	void nextPermutation() {
		if (!std::next_permutation(permutation.begin(), permutation.end())) {
			done = true;
		}
	}


	bool isEnd() const {
		return done;
	}

	void reset() {
		done = false;
	}

private:
	permutation_type permutation;
	bool done;
};

}

#endif /* PERMUTATIONS_HPP */
