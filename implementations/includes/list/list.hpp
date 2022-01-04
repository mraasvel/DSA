#ifndef LIST_HPP
# define LIST_HPP

#include "is_iterator.hpp"
#include <memory> // std::allocator

/*
Class Structure:

	Private Node + Iterator definitions
	Public types
	Private types

*/

namespace DS {

template <class T, class Allocator = std::allocator<T> >
class list {
/* Template Argument Validation */
	static_assert(std::is_same<T, typename Allocator::value_type>::value, "allocator requirement violation");
private:
	struct ListNodeBase {
		ListNodeBase()
		: prev(nullptr), next(nullptr) {}
		ListNodeBase* prev;
		ListNodeBase* next;
	};

	struct ListNode : public ListNodeBase {
		ListNode()
		: ListNodeBase() {}
		explicit ListNode(const T& value)
		: ListNodeBase(), value(value) {}
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
Constructors */

	list() {
		init();
	}

	explicit list(const allocator_type& alloc)
	: value_allocator(alloc) {
		init();
	}

	list(size_type count, const value_type& value, const allocator_type& alloc = allocator_type())
	: list(alloc) {
		insert(cend(), count, value);
	}

	explicit list(size_type count)
	: list() {
		insert(cend(), count);
	}

	template <class InputIt,
		typename = RequireInputIterator<InputIt>>
	list(InputIt first, InputIt last, const allocator_type& alloc = allocator_type())
	: list(alloc) {
		insert(cend(), first, last);
	}

	list(const list& other)
	: list(other, std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.get_allocator())) {}

	list(const list& other, const allocator_type& alloc)
	: value_allocator(alloc) {
		init();
		insert(cend(), other.begin(), other.end());
	}

	// TODO: call swap function
	list(list&& other)
	: _size(0), start(nullptr), value_allocator(std::move(other.value_allocator)) {
		std::swap(start, other.start);
		std::swap(_size, other._size);
	}

	// TODO: call swap function
	// TODO: SFINAE to check allocator equality: so that copy constructor is not necessary (insert is not instantiated)
	list(list&& other, const allocator_type& alloc)
	: _size(0), start(nullptr) {
		if (alloc != other.get_allocator()) {
			init();
			insert(cend(), other.begin(), other.end());
		} else {
			std::swap(start, other.start);
			std::swap(_size, other._size);
		}

	}

	list(std::initializer_list<T> init, const allocator_type& alloc = allocator_type())
	: list(alloc) {
		insert(cend(), init);
	}

/*
Destructor */

	~list() {
		fullClear();
	}
/*
Assignment */

	// TODO: use old allocator to clear memory, new allocator to assign
	list& operator=(const list& other) {
		if (this == &other) {
			return *this;
		}
		if (std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value) {
			if (value_allocator != other.value_allocator) {
				clear();
			}
			value_allocator = other.value_allocator;
			node_allocator = other.node_allocator;
			nodebase_allocator = other.nodebase_allocator;
		}
		assign(other.begin(), other.end());
		return *this;
	}

	list& operator=(list&& other) {
		fullClear();
		std::swap(start, other.start);
		std::swap(_size, other.size);
		return *this;
	}

	list& operator=(std::initializer_list<value_type> ilist) {
		assign(ilist);
		return *this;
	}

	void assign(size_type count, const value_type& value) {
		clear();
		insert(cend(), count, value);
	}

	template <class InputIt,
		typename = RequireInputIterator<InputIt>>
	void assign(InputIt first, InputIt last) {
		clear();
		insert(cend(), first, last);
	}

	void assign(std::initializer_list<value_type> ilist) {
		assign(ilist.begin(), ilist.end());
	}

	allocator_type get_allocator() const noexcept {
		return value_allocator;
	}

/*
Element Access */

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
Iterating */

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
Capacity */

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
Modifiers */

	// TODO: replace with erase() so that the loop invariant is not broken
	void clear() noexcept {
		iterator it = begin();
		iterator ite = end();
		while (it != ite) {
			destroyNode(it++);
		}
		_size = 0;
		start = ite.base();
		resetSentinel();
	}

	iterator insert(const_iterator pos, const value_type& value) {
		NodeBase* entry = newNode(value);
		insertNode(pos, entry);
		return iterator(entry);
	}

	iterator insert(const_iterator pos, value_type&& value) {
		return insert(pos, value);
	}

	iterator insert(const_iterator pos, size_type count, const value_type& value) {
		iterator it = iterator(pos.base());
		while (count-- > 0) {
			it = insert(it, value);
		}
		return it;
	}

	template <class InputIt,
		typename = RequireInputIterator<InputIt>>
	iterator insert(const_iterator pos, InputIt first, InputIt last) {
		if (first == last) {
			return iterator(pos.base());
		}
		iterator it = insert(pos, *first++);
		while (first != last) {
			insert(pos, *first);
			first++;
		}
		return it;
	}

	iterator insert(const_iterator pos, std::initializer_list<value_type> ilist) {
		iterator it = iterator(pos.base());
		for (const value_type& value : ilist) {
			if (it == pos) {
				it = insert(pos, value);
			} else {
				insert(pos, value);
			}
		}
		return it;
	}

private:
/*
Allocation */

	void init() {
		_size = 0;
		start = nullptr;
		start = newNodeBase();
		resetSentinel();
	}

	void resetSentinel() noexcept {
		start->next = start;
		start->prev = start;
	}

	NodeBase* newNode(const value_type& value) {
		Node* node = node_allocator.allocate(1);
		try {
			node_allocator.construct(node, value);
		} catch (...) {
			node_allocator.deallocate(node, 1);
		}
		return node;
	}

	NodeBase* newDefaultNode() {
		Node* node = node_allocator.allocate(1);
		try {
			node_allocator.construct(node);
		} catch(...) {
			node_allocator.deallocate(node, 1);
		}
		return node;
	}

	NodeBase* newNodeBase() {
		NodeBase* node = nodebase_allocator.allocate(1);
		try {
			nodebase_allocator.construct(node);
		} catch (...) {
			nodebase_allocator.deallocate(node, 1);
		}
		return node;
	}
/*
Modification */

	iterator insert(const_iterator pos, size_type count) {
		iterator it = iterator(pos.base());
		while (count-- > 0) {
			NodeBase* node = newDefaultNode();
			it = insertNode(it, node);
		}
		return it;
	}

	iterator insertNode(const_iterator pos, NodeBase* node) {
		NodeBase* next = pos.base();
		node->prev = next->prev;
		node->next = next;
		next->prev->next = node;
		next->prev = node;
		if (pos == begin()) {
			start = node;
		}
		_size += 1;
		return iterator(node);
	}

/*
Destruction */
	void fullClear() noexcept {
		if (start == nullptr) {
			return;
		}
		clear();
		destroySentinel();
		start = nullptr;
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
	allocator_type value_allocator;
	nodebase_allocator_type nodebase_allocator;
	node_allocator_type node_allocator;
};

}

#endif /* LIST_HPP */
