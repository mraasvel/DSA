#ifndef LIST_HPP
# define LIST_HPP

#include <memory> // std::allocator

/*
Class Structure:

	Private Node + Iterator definitions
	Public types
	Private types

*/

namespace DataStructures {

template <class T, class Allocator = std::allocator<T> >
class list {
private:
	template <typename V>
	struct ListNodeBase {
		ListNodeBase()
		: prev(nullptr), next(nullptr) {}
		ListNodeBase* prev;
		ListNodeBase* next;
	};

	template <typename V>
	struct ListNode : public ListNodeBase<V> {
		explicit ListNode(const V& value)
		: value(value) {}
		V value;
	};

	template <typename V>
	class ListIterator {
	private:
		typedef V value_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef ListNode<V>* NodePointer;
		typedef ListNodeBase<V>* BasePointer;
		typedef ListNodeBase<const V>* ConstBasePointer;
		typedef ListIterator<const V> const_iterator;
	public:
		ListIterator()
		: base(nullptr) {}
		explicit ListIterator(BasePointer base)
		: base(base) {}

		ListIterator& operator++() {
			base = base->next;
			return *this;
		}

		ListIterator& operator++(int) {
			ListIterator copy(*this);
			base = base->next;
			return copy;
		}

		ListIterator& operator--() {
			base = base->prev;
			return *this;
		}

		ListIterator& operator--(int) {
			ListIterator copy(*this);
			base = base->prev;
			return copy;
		}

		reference operator*() const {
			return reinterpret_cast<NodePointer>(base)->value;
		}

		pointer operator->() const {
			return &(operator*());
		}

		operator const_iterator() const {
			return const_iterator(reinterpret_cast<ConstBasePointer>(base));
		}

		bool operator==(const ListIterator& rhs) const {
			return base == rhs.base;
		}

		bool operator!=(const ListIterator& rhs) const {
			return !(*this == rhs);
		}

	private:
		BasePointer base;
	};

public:
	typedef T value_type;
	typedef Allocator allocator_type;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef typename std::allocator_traits<allocator_type>::pointer pointer;
	typedef typename std::allocator_traits<Allocator>::const_pointer const_pointer;

	typedef ListIterator<value_type> iterator;
	typedef ListIterator<const value_type> const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

private:
	typedef ListNodeBase<value_type> NodeBase;
	typedef ListNode<value_type> Node;
	typedef typename allocator_type::template rebind<NodeBase>::other nodebase_allocator_type;
	typedef typename allocator_type::template rebind<Node>::other node_allocator_type;
public:
private:
};

}

#endif /* LIST_HPP */
