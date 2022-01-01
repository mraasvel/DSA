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
	struct ListNodeBase {
		ListNodeBase()
		: prev(nullptr), next(nullptr) {}
		ListNodeBase* prev;
		ListNodeBase* next;
	};

	struct ListNode : public ListNodeBase {
		explicit ListNode(const T& value)
		: value(value) {}
		T value;
	};

	template <typename U>
	class ListIterator {
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = U;
		using pointer = value_type*;
		using reference = value_type&;
		using iterator_category = std::bidirectional_iterator_tag;
	private:
		using NodePointer = ListNode*;
		using BasePointer = ListNodeBase*;
		using const_iterator = ListIterator<const value_type>;
	public:
		ListIterator()
		: node(nullptr) {}
		explicit ListIterator(BasePointer node)
		: node(node) {}

		ListIterator& operator++() {
			node = node->next;
			return *this;
		}

		ListIterator operator++(int) {
			ListIterator copy = *this;
			node = node->next;
			return copy;
		}

		ListIterator& operator--() {
			node = node->prev;
			return *this;
		}

		ListIterator operator--(int) {
			ListIterator copy = *this;
			node = node->prev;
			return copy;
		}

		reference operator*() const {
			return static_cast<NodePointer>(node)->value;
		}

		pointer operator->() const {
			return &(operator*());
		}

		friend bool operator==(const ListIterator& a, const ListIterator& b) {
			return a.node == b.node;
		}

		friend bool operator!=(const ListIterator& a, const ListIterator& b) {
			return !(a == b);
		}

		operator const_iterator() const {
			return const_iterator(node);
		}

		BasePointer base() const {
			return node;
		}

	private:
		BasePointer node;
	};

public:
	using value_type = T;
	using allocator_type = Allocator;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = typename std::allocator_traits<allocator_type>::pointer;
	using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;

	using iterator = ListIterator<value_type>;
	using const_iterator = ListIterator<const value_type>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
	using NodeBase = ListNodeBase;
	using Node = ListNode;
	using nodebase_allocator_type = typename allocator_type::template rebind<NodeBase>::other;
	using node_allocator_type = typename allocator_type::template rebind<Node>::other;
public:
/*
Constructor
*/
	list() {
		init();
	}

	explicit list(const allocator_type& alloc)
	: list() {}

	list(size_type count, const value_type& value, const allocator_type& alloc);
	explicit list(size_type count);
	template <class InputIt>
	list(InputIt first, InputIt last, const allocator_type& alloc = allocator_type());
	list(const list& other);
	list(const list& other, const allocator_type& alloc);
	list(list&& other);
	list(list&& other, const allocator_type& alloc);
	list(std::initializer_list<T> init, const allocator_type& alloc = allocator_type());
/*
Destructor
*/
	~list() {
		if (start == nullptr) {
			return;
		}
		clear();
		destroySentinel();
	}
/*
Assignment
*/
	list& operator=(const list& other);
	list& operator=(list&& other);
	list& operator=(std::initializer_list<T> ilist);

	void assign(size_type count, const T& value);
	template <class InputIt>
	void assign(InputIt first, InputIt last);
	void assign(std::initializer_list<T> ilist);

	allocator_type get_allocator() const noexcept {
		return allocator_type();
	}

/*
Element Access
*/
	reference front() {
		return *begin();
	}

	const_reference front() const {
		return *cbegin();
	}

	reference back() {
		return *std::prev(begin());
	}

	const_reference back() const {
		return *std::prev(cbegin());
	}
/*
Iterating
*/
	iterator begin() noexcept {
		return iterator(start);
	}

	const_iterator begin() const noexcept {
		return const_iterator(start);
	}

	const_iterator cbegin() const noexcept {
		return const_iterator(start);
	}

	iterator end() noexcept {
		return iterator(std::prev(begin()));
	}

	const_iterator end() const noexcept {
		return const_iterator(std::prev(begin()));
	}

	const_iterator cend() const noexcept {
		return const_iterator(std::prev(begin()));
	}

	reverse_iterator rbegin() noexcept {
		return reverse_iterator(end());
	}

	const_reverse_iterator rbegin() const noexcept {
		return const_reverse_iterator(end());
	}

	const_reverse_iterator crbegin() const noexcept {
		return const_reverse_iterator(end());
	}

	reverse_iterator rend() noexcept {
		return reverse_iterator(begin());
	}

	const_reverse_iterator rend() const noexcept {
		return const_reverse_iterator(begin());
	}

	const_reverse_iterator crend() const noexcept {
		return const_reverse_iterator(begin());
	}
/*
Capacity
*/
	bool empty() const noexcept {
		return cbegin() == cend();
	}

	size_type size() const noexcept {
		return _size;
	}

	size_type max_size() const noexcept {
		return node_allocator.max_size();
	}
/*
Modifiers
*/
	void clear() noexcept {
		iterator it = begin();
		while (it != end()) {
			destroyNode(it++);
		}
		resetSentinel();
	}

private:
	void init() {
		_size = 0;
		start = nullptr;
		start = nodebase_allocator.allocate(1);
		resetSentinel();
	}

	void resetSentinel() noexcept {
		start->next = start;
		start->prev = start;
	}

	void destroySentinel() noexcept {
		destroyBaseNode(end());
	}

	void destroyBaseNode(iterator it) noexcept {
		nodebase_allocator.destroy(it.base());
		nodebase_allocator.deallocate(it.base(), 1);
	}

	void destroyNode(iterator it) noexcept {
		Node* ptr = static_cast<Node*>(it.base());
		node_allocator.destroy(ptr);
		node_allocator.deallocate(ptr, 1);
	}

private:
	size_type _size;
	NodeBase* start;
	nodebase_allocator_type nodebase_allocator;
	node_allocator_type node_allocator;
};

}

#endif /* LIST_HPP */
