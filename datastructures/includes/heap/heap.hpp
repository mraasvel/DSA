#ifndef HEAP_HPP
#define HEAP_HPP

#include <vector>
#include <functional>

namespace DSA {

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
	using compare_type		= Compare;
	using value_type		= typename Container::value_type;
	using reference			= typename Container::reference;
	using const_reference	= typename Container::const_reference;
	using size_type			= typename Container::size_type;
public:
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
	}

	void push(value_type&& v) {
		c.push_back(std::move(v));
		heapifyUp();
	}

	template <typename... Args>
	void emplace(Args&&... args) {
		c.emplace_back(std::forward<Args>(args)...);
		heapifyUp();
	}

	void pop() {
		std::swap(c.front(), c.back());
		c.pop_back();
		heapifyDown();
	}

	void swap(Heap& other) {
		std::swap(c, other.c);
		std::swap(comp, other.comp);
	}

	void print() const {
		std::cout << "HEAP:";
		for (const auto& v : c) {
			std::cout << ' ' << v;
		}
		std::cout << std::endl;
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

private:
	container_type c;
	compare_type comp;
};


}

#endif /* HEAP_HPP */
