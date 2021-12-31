#ifndef REDBLACK_ITERATOR_HPP
# define REDBLACK_ITERATOR_HPP

# include "redblack_node.hpp"
# include <iterator>

template <typename T>
class RedBlackIterator {
private:
	typedef std::iterator<std::bidirectional_iterator_tag, T> iterator_t;
	typedef typename RedBlackNode<T>::node_type node_type;
	typedef typename node_type::const_node_type const_node_type;
	typedef typename node_type::node_pointer node_pointer;
	typedef typename node_type::const_node_pointer const_node_pointer;

	typedef RedBlackIterator<const T> const_iterator;
public:
	typedef typename iterator_t::iterator_category iterator_category;
	typedef typename iterator_t::value_type value_type;
	typedef typename iterator_t::difference_type difference_type;
	typedef typename iterator_t::pointer pointer;
	typedef typename iterator_t::reference reference;

	typedef const pointer const_pointer;
	typedef const reference const_reference;
public:

	RedBlackIterator(node_pointer p, node_pointer end, bool end_reached)
	: ptr(p), end_ptr(end), end_reached(end_reached) {}

	RedBlackIterator()
	: ptr(NULL), end_ptr(NULL), end_reached(false) {}

	RedBlackIterator(const RedBlackIterator& rhs)
	: ptr(rhs.ptr), end_ptr(rhs.end_ptr), end_reached(rhs.end_reached) {}

	~RedBlackIterator() {}

	RedBlackIterator& operator=(const RedBlackIterator& rhs) {
		if (this == &rhs) {
			return *this;
		}

		ptr = rhs.ptr;
		end_ptr = rhs.end_ptr;
		end_reached = rhs.end_reached;
		return *this;
	}

/* Operations */

/* Input Iterator */
	RedBlackIterator& operator++() {
		ptr = ptr->nextNode();
		if (ptr == NULL) {
			end_reached = true;
		}
		return *this;
	}

	RedBlackIterator operator++(int) {
		RedBlackIterator temp(*this);
		this->operator++();
		return temp;
	}

	template <typename T1, typename T2>
	friend bool operator==(const RedBlackIterator<T1>& lhs, const RedBlackIterator<T2>& rhs);

	reference operator*() {
		return ptr->getValue();
	}

	const_reference operator*() const {
		return ptr->getValue();
	}

	pointer operator->() {
		return &(ptr->getValue());
	}

	const_pointer operator->() const {
		return &(ptr->getValue());
	}

/* Bidirectional */

	RedBlackIterator& operator--() {
		if (end_reached) {
			ptr = end_ptr->left;
			end_reached = false;
		} else {
			ptr = ptr->prevNode();
		}
		return *this;
	}

	RedBlackIterator operator--(int) {
		RedBlackIterator temp(*this);
		this->operator--();
		return temp;
	}

/* Other */
	operator const_iterator() const {
		return const_iterator(reinterpret_base(), reinterpret_end(), end_reached);
	}

	node_pointer base() {
		return ptr;
	}

	const_node_pointer base() const {
		return reinterpret_base();
	}

private:
	node_pointer end() {
		return end_ptr;
	}

	const_node_pointer end() const {
		return reinterpret_end();
	}

	const_node_pointer reinterpret_base() const {
		return reinterpret_cast<const_node_pointer> (ptr);
	}

	const_node_pointer reinterpret_end() const {
		return reinterpret_cast<const_node_pointer> (end_ptr);
	}

private:
	node_pointer ptr;
	node_pointer end_ptr;
	bool end_reached;
};

template <typename T1, typename T2>
bool operator==(const RedBlackIterator<T1>& lhs, const RedBlackIterator<T2>& rhs) {
	return lhs.base() == rhs.base()
	&& lhs.end() == rhs.end()
	&& lhs.end_reached == rhs.end_reached;
}

template <typename T1, typename T2>
bool operator!=(const RedBlackIterator<T1>& lhs, const RedBlackIterator<T2>& rhs) {
	return !(lhs == rhs);
}

#endif /* REDBLACK_ITERATOR_HPP */
