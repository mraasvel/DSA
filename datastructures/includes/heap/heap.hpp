#ifndef HEAP_HPP
#define HEAP_HPP

#include "sfinae.hpp"
#include <vector>
#include <functional>
#include <algorithm>

namespace DSA {

template <class RandomIt, class Compare,
	RequireRandomAccessIterator<RandomIt> = true>
void make_heap(RandomIt first, RandomIt last,
				Compare comp) {
	
}

template <class RandomIt>
void make_heap(RandomIt first, RandomIt last) {
	make_heap(first, last, std::less<decltype(*first)>());
}

template <
	typename T,
	typename Container = std::vector<T>,
	typename Compare = std::less<typename Container::value_type>
>
class Heap {
public:
	static_assert(std::is_same<T, typename Container::value_type>::value, "invalid container");
public:
	using container_type	= Container;
	using value_compare		= Compare;
	using value_type		= typename Container::value_type;
	using size_type			= typename Container::size_type;
	using reference			= typename Container::reference;
	using const_reference	= typename Container::const_reference;

public:
	Heap()
	: Heap(Compare(), Container()) {}
	explicit Heap(const Compare& compare)
	: Heap(compare, Container()) {}

	Heap(const Compare& compare, const Container& cont)
	: comp(compare), c(cont) {
		std::make_heap(c.begin(), c.end(), comp);
	}

	Heap(const Compare& compare,Container&& cont)
	: comp(compare), c(std::move(cont)) {
		std::make_heap(c.begin(), c.end(), comp);
	}

	Heap(const Heap& other) = default;
	Heap(Heap&& other) = default;

	template <class InputIt,
		RequireInputIterator<InputIt> = true>
	Heap(InputIt first, InputIt last,
			const Compare& compare = Compare())
	: comp(compare), c(first, last) {
		std::make_heap(c.begin(), c.end(), comp);
	}

	template <class InputIt,
		RequireInputIterator<InputIt> = true>
	Heap(InputIt first, InputIt last,
		const Compare& compare, const Container& cont)
	: comp(compare), c(cont) {
		c.insert(c.end(), first, last);
		std::make_heap(c.begin(), c.end(), comp);
	}

	template <class InputIt,
		RequireInputIterator<InputIt> = true>
	Heap(InputIt first, InputIt last,
		const Compare& compare, Container&& cont)
	: comp(compare), c(std::move(cont)) {
		c.insert(c.end(), first, last);
		std::make_heap(c.begin(), c.end(), comp);
	}

	template <class Alloc>
	explicit Heap(const Alloc& alloc)
	: Heap(Compare(), alloc) {}

	template <class Alloc,
		RequireAllocator<container_type, Alloc> = true>
	Heap(const Compare& compare, const Alloc& alloc)
	: comp(compare), c(alloc) {}

	template <class Alloc,
		RequireAllocator<container_type, Alloc> = true>
	Heap(const Compare& compare, const Container& cont,
		const Alloc& alloc)
	: comp(compare), c(cont, alloc) {
		std::make_heap(c.begin(), c.end(), comp);
	}

	template <class Alloc,
		RequireAllocator<container_type, Alloc> = true>
	Heap(const Compare& compare, Container&& cont,
		const Alloc& alloc)
	: comp(compare), c(std::move(cont), alloc) {
		std::make_heap(c.begin(), c.end(), comp);
	}

	template <class Alloc,
		RequireAllocator<container_type, Alloc> = true>
	Heap(const Heap& other, const Alloc& alloc)
	: comp(other.comp), c(other.c, alloc) {}

	template <class Alloc,
		RequireAllocator<container_type, Alloc> = true>
	Heap(Heap&& other, const Alloc& alloc)
	: comp(std::move(other.comp)), c(std::move(other.c), alloc) {}

	template <class InputIt, class Alloc,
		RequireInputIterator<InputIt> = true,
		RequireAllocator<container_type, Alloc> = true>
	Heap(InputIt first, InputIt last, const Compare& compare,
		const Alloc& alloc)
	: comp(compare), c(first, last, alloc) {
		std::make_heap(c.begin(), c.end(), comp);
	}

	template <class InputIt, class Alloc,
		RequireInputIterator<InputIt> = true,
		RequireAllocator<container_type, Alloc> = true>
	Heap(InputIt first, InputIt last, const Compare& compare,
		const Container& cont, const Alloc& alloc)
	: comp(compare), c(cont, alloc) {
		c.insert(c.end(), first, last);
		std::make_heap(c.begin(), c.end(), comp);
	}

	template <class InputIt, class Alloc,
		RequireInputIterator<InputIt> = true,
		RequireAllocator<container_type, Alloc> = true>
	Heap(InputIt first, InputIt last, const Compare& compare,
		Container&& cont, const Alloc& alloc)
	: comp(compare), c(std::move(cont), alloc) {
		c.insert(c.end(), first, last);
		std::make_heap(c.begin(), c.end(), comp);
	}

	~Heap() {}

	Heap& operator=(const Heap& other) = default;
	Heap& operator=(Heap&& other) = default;

	const_reference top() const {
		return c.front();
	}

	bool empty() const {
		return c.empty();
	}

	size_type size() const {
		return c.size();
	}

	void push(const value_type& v) {
		c.push_back(v);
		heapifyUp();
		// std::push_heap(c.begin(), c.end(), comp);
	}

	void push(value_type&& v) {
		c.push_back(std::move(v));
		heapifyUp();
		// std::push_heap(c.begin(), c.end(), comp);
	}

	template <typename... Args>
	void emplace(Args&&... args) {
		c.emplace_back(std::forward<Args>(args)...);
		heapifyUp();
		// std::push_heap(c.begin(), c.end(), comp);
	}

	void pop() {
		std::swap(c.front(), c.back());
		c.pop_back();
		heapifyDown();
		// std::pop_heap(c.begin(), c.end(), comp);
		// c.pop_back();
	}

	void swap(Heap& other) noexcept {
		std::swap(c, other.c);
		std::swap(comp, other.comp);
	}

private:
	void heapifyUp() {
		// bubble up
		size_type index = c.size() - 1;
		size_type parent = parentIndex(index);
		while (index != 0 && greater(c[index], c[parent])) {
			std::swap(c[index], c[parent]);
			index = parent;
			parent = parentIndex(index);
		}
	}

	void heapifyDown() {
		size_type index = 0;
		// bubble down: swap with the largest child
		while (true) {
			auto left = leftChildIndex(index);
			auto right = rightChildIndex(index);
			// Condition: no children or current node is already greater than it's children
			if (left >= size() ||
				(greater(c[index], c[left]) &&
				(right >= size() || greater(c[index], c[right])))
			) {
				break;
			} else if (right >= size() || greater(c[left], c[right])) {
				std::swap(c[index], c[left]);
				index = left;
			} else {
				std::swap(c[index], c[right]);
				index = right;
			}
		}
	}

/*
Utils */
	size_type parentIndex(size_type index) const {
		return (index - 1) / 2;
	}

	size_type leftChildIndex(size_type index) const {
		return index * 2 + 1;
	}

	size_type rightChildIndex(size_type index) const {
		return index * 2 + 2;
	}

	bool greater(const value_type& a, const value_type& b) const {
		return comp(b, a);
	}

	bool greaterEqual(const value_type& a, const value_type& b) const {
		return !(comp(a, b));
	}

protected:
	value_compare comp;
	container_type c;
};

}

namespace std {

template <class T, class Container, class Compare>
void swap(  DSA::Heap<T, Container, Compare>& lhs,
			DSA::Heap<T, Container, Compare>& rhs) {
	lhs.swap(rhs);
}

}

#endif /* HEAP_HPP */
