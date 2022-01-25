#ifndef HEAP_HPP
#define HEAP_HPP

#include <vector>
#include <functional>

namespace DSA {

template <
	typename T,
	typename Compare = std::less<T>,
	typename Alloc = std::allocator<T>,
	typename C = std::vector<T, Alloc>
>
class Heap {
public:
	using value_type = T;
	using compare_type = Compare;
	using container_type = C;
	using allocator_type = Alloc;

	using iterator = container_type::iterator;
	using const_iterator = container_type::const_iterator;
	using reverse_iterator = container_type::reverse_iterator;
	using const_reverse_iterator = container_type::const_reverse_iterator;

public:
	Heap() {}
	~Heap() {}

/* iterators */
	iterator begin() {
		return c.begin();
	}
	const_iterator begin() const {
		return c.begin();
	}
	const_iterator cbegin() const {
		return c.begin();
	}

	iterator end() {
		return c.end();
	}
	const_iterator end() const {
		return c.end();
	}
	const_iterator end() const {
		return c.end();
	}

	reverse_iterator rbegin() {
		return c.rbegin();
	}
	const_reverse_iterator rbegin() const {
		return c.rbegin();
	}
	const_reverse_iterator crbegin() const {
		return c.rbegin();
	}

	reverse_iterator rend() {
		return c.rend();
	}
	const_reverse_iterator rend() const {
		return c.rend();
	}
	const_reverse_iterator crend() const {
		return c.rend();
	}

	void push(const T& value);
	void pop();
	const T& top() const {
		return c.front();
	}

private:
	container_type c;
};


}

#endif /* HEAP_HPP */
