#ifndef LIST_HPP
# define LIST_HPP

#include "is_iterator.hpp"
#include <memory> // std::allocator
#include <iostream> // REMOVE
#include <cassert> // REMOVE

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

		explicit ListNode(T&& value)
		: ListNodeBase(), value(std::move(value)) {}

		template <class... Args>
		explicit ListNode(Args&& ...args)
		: value(std::forward<Args>(args)...) {}

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
		insertDefault(cend(), count);
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

	list(list&& other)
	: _size(0), start(nullptr), value_allocator(std::move(other.value_allocator)) {
		std::swap(start, other.start);
		std::swap(_size, other._size);
	}

	// TODO: SFINAE to check allocator equality: so that copy constructor is not necessary (insert is not instantiated)
	list(list&& other, const allocator_type& alloc)
	: _size(0), start(nullptr), value_allocator(alloc) {
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
Assignment, Copy Assignment Operators */

	/*
	If std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value is true,
	the allocator of *this is replaced by a copy of that of other.

	If the allocator of *this after assignment would compare unequal to its old value,
	the old allocator is used to deallocate the memory,
	then the new allocator is used to allocate it before copying the elements.

	Otherwise, the memory owned by *this may be reused when possible.
	In any case, the elements originally belong to *this may be either destroyed or replaced by element-wise copy-assignment. */
	list& operator=(const list& other) {
		if (this == &other) {
			return *this;
		}
		if (std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value) {
			propagateAllocatorsOnCopy(other);
		}
		assignReuseMem(other);
		return *this;
	}

	/*
	If std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value is true,
	the allocator of *this is replaced by a copy of that of other.

	If it is false and the allocators of *this and other do not compare equal,
	*this cannot take ownership of the memory owned by other and must move-assign each element individually,
	allocating additional memory using its own allocator as needed.

	In any case, all elements originally belong to
	*this are either destroyed or replaced by element-wise move-assignment. */
	list& operator=(list&& other) {
		if (std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value) {
			fullClear();
			propagateAllocators(other);
		} else if (!equalAllocators(other)) {
			init();
			assignReuseMem(other);
			return *this;
		}
		// TODO: call swap function
		std::swap(start, other.start);
		std::swap(_size, other._size);
		return *this;
	}

	list& operator=(std::initializer_list<value_type> ilist) {
		assign(ilist);
		return *this;
	}

	void assign(size_type count, const value_type& value) {
		assignReuseMem(count, value);
	}

	template <class InputIt,
		typename = RequireInputIterator<InputIt>>
	void assign(InputIt first, InputIt last) {
		assignReuseMem(first, last);
	}

	void assign(std::initializer_list<value_type> ilist) {
		assignReuseMem(ilist.begin(), ilist.end());
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
		return *std::prev(end());
	}

	const_reference back() const {
		return *std::prev(cend());
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

	void clear() noexcept {
		erase(begin(), end());
	}

	iterator insert(const_iterator pos, const value_type& value) {
		return insertNode(pos, newNode(value));
	}

	iterator insert(const_iterator pos, value_type&& value) {
		return insertNode(pos, newNode(std::move(value)));
	}

	iterator insert(const_iterator pos, size_type count, const value_type& value) {
		iterator it {pos.base()};
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
		iterator it {emplace(pos, *first++)};
		while (first != last) {
			emplace(pos, *first);
			first++;
		}
		return it;
	}

	iterator insert(const_iterator pos, std::initializer_list<value_type> ilist) {
		iterator it {pos.base()};
		for (const value_type& value : ilist) {
			if (it == pos) {
				it = emplace(pos, value);
			} else {
				emplace(pos, value);
			}
		}
		return it;
	}

	template <class... Args>
	iterator emplace(const_iterator pos, Args&&... args) {
		return insertNode(pos, newNode(std::forward<Args>(args)...));
	}

	/*
	1. Cut link
	2. Save previous node
	3. Destroy node */
	iterator erase(const_iterator pos) {
		iterator next = unlink(pos);
		_size -= 1;
		destroyNode(iterator(pos.base()));
		return next;
	}

	iterator erase(const_iterator first, const_iterator last) {
		while (first != last) {
			first = erase(first);
		}
		return iterator(first.base());
	}

	void push_back(const value_type& value) {
		insert(cend(), value);
	}

	void push_back(value_type&& value) {
		insert(cend(), std::move(value));
	}

	template <class... Args>
	void emplace_back(Args&&... args) {
		emplace(cend(), std::forward<Args>(args)...);
	}

	void pop_back() {
		erase(cend());
	}

	void push_front(const value_type& value) {
		insert(cbegin(), value);
	}

	void push_front(value_type&& value) {
		insert(cbegin(), std::move(value));
	}

	template <class... Args>
	void emplace_front(Args&&... args) {
		emplace(cbegin(), std::forward<Args>(args)...);
	}

	void pop_front() {
		erase(cbegin());
	}

	void resize(size_type count) {
		if (size() < count) {
			insertDefault(cend(), count - size());
		} else {
			erase(size() - count);
		}
	}

	void resize(size_type count, const value_type& value) {
		if (size() < count) {
			insert(cend(), count - size(), value);
		} else {
			erase(size() - count);
		}
	}

	void swap(list& other) {
		if (std::allocator_traits<allocator_type>::propagate_on_container_swap::value) {
			swapAllocators(other);
		}
		std::swap(start, other.start);
		std::swap(_size, other._size);
	}

/*
Operations */

	void merge(list& other) {
		merge(other, std::less<value_type>());
	}

	void merge(list&& other) {
		merge(other);
	}

	template <class Compare>
	void merge(list& other, Compare comp) {
		if (this == &other) {
			return;
		}
		auto it_own = begin();
		auto it_other = other.begin();
		while (it_own != end() && it_other != other.end()) {
			if (comp(*it_other, *it_own)) {
				splice(it_own, other, it_other++);
			} else {
				++it_own;
			}
		}
		if (other.size() != 0) {
			splice(end(), other);
		}
	}

	template <class Compare>
	void merge(list&& other, Compare comp) {
		merge(other, comp);
	}

	void splice(const_iterator pos, list& other) {
		if (other.size() == 0) {
			return;
		}
		auto first = other.begin();
		auto last = std::prev(other.end());
		other.unlinkRange(first, other.end());
		insertRange(pos, first, last, other.size());
		other._size = 0;
	}

	void splice(const_iterator pos, list&& other) {
		splice(pos, other);
	}

	void splice(const_iterator pos, list& other, const_iterator it) {
		other.unlink(it);
		other._size -= 1;
		insertRange(pos, it, it, 1);
	}

	void splice(const_iterator pos, list&& other, const_iterator it) {
		splice(pos, other, it);
	}

	void splice(const_iterator pos, list& other,
				const_iterator first, const_iterator last) {
		ptrdiff_t delta {0};
		if (this != &other) {
			delta = std::distance(first, last);
		}
		spliceDelta(pos, other, first, last, delta);
	}

	void splice(const_iterator pos, list&& other,
				const_iterator first, const_iterator last) {
		splice(pos, other, first, last);
	}

	void remove(const value_type& value) {
		return remove_if([&value] (const value_type& x) -> bool {
			return x == value;
		});
	}

	template <class UnaryPredicate>
	void remove_if(UnaryPredicate pred) {
		auto it = cbegin();
		while (it != cend()) {
			if (pred(*it)) {
				erase(it++);
			} else {
				++it;
			}
		}
	}

	void reverse() noexcept {
		start = std::prev(cend()).base();
		auto it = cbegin();
		while (it != cend()) {
			std::swap(it.base()->prev, it.base()->next);
			++it;
		}
		std::swap(it.base()->prev, it.base()->next);
	}

	void unique() {
		return unique([] (const value_type& a, const value_type& b) -> bool {
			return a == b;
		});
	}

	template <class BinaryPredicate>
	void unique(BinaryPredicate pred) {
		auto it = cbegin();
		while (it != cend()) {
			auto next {std::next(it)};
			if (next == end()) {
				break;
			} else if (pred(*it, *next)) {
				erase(next);
			} else {
				it = next;
			}
		}
	}

	void sort() {
		sort(std::less<value_type>());
	}

	template <class Compare>
	void sort(Compare comp) {
		mergesort(*this, comp);
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
		Node* node {node_allocator.allocate(1)};
		try {
			node_allocator.construct(node, value);
		} catch (...) {
			node_allocator.deallocate(node, 1);
		}
		return node;
	}

	NodeBase* newNode(value_type&& value) {
		Node* node {node_allocator.allocate(1)};
		try {
			node_allocator.construct(node, std::move(value));
		} catch (...) {
			node_allocator.deallocate(node, 1);
		}
		return node;
	}

	template <class... Args>
	NodeBase* newNode(Args&& ...args) {
		Node* node {node_allocator.allocate(1)};
		try {
			node_allocator.construct(node, std::forward<Args>(args)...);
		} catch (...) {
			node_allocator.deallocate(node, 1);
		}
		return node;
	}

	NodeBase* newDefaultNode() {
		Node* node {node_allocator.allocate(1)};
		try {
			node_allocator.construct(node);
		} catch(...) {
			node_allocator.deallocate(node, 1);
		}
		return node;
	}

	NodeBase* newNodeBase() {
		NodeBase* node {nodebase_allocator.allocate(1)};
		try {
			nodebase_allocator.construct(node);
		} catch (...) {
			nodebase_allocator.deallocate(node, 1);
		}
		return node;
	}

/*
Assignation */

	bool equalAllocators(const list& other) {
		return value_allocator == other.value_allocator
			&& node_allocator == other.node_allocator
			&& nodebase_allocator == other.node_allocator;
	}

	void propagateAllocatorsOnCopy(const list& other) {
		if (!equalAllocators(other)) {
			fullClear();
		}
		propagateAllocators(other);
		if (start == nullptr) {
			init();
		}
	}

	void propagateAllocators(const list& other) {
		value_allocator = other.value_allocator;
		node_allocator = other.node_allocator;
		nodebase_allocator = other.nodebase_allocator;
	}

	void swapAllocators(list& other) {
		std::swap(value_allocator, other.value_allocator);
		std::swap(node_allocator, other.node_allocator);
		std::swap(nodebase_allocator, other.nodebase_allocator);
	}

	/*
	For copy assignment: list has same allocator so we can reuse allocated nodes. */
	void assignReuseMem(const list& other) {
		assignReuseMem(other.begin(), other.end());
	}

	void assignReuseMem(list&& other) {
		assignReuseMem(std::move_iterator<iterator>{other.begin()},
					std::move_iterator<iterator>{other.end()});
	}

	template <typename InputIt,
		typename = RequireInputIterator<InputIt>>
	void assignReuseMem(InputIt first, InputIt last) {
		iterator it = begin();
		while (first != last) {
			if (it != end()) {
				*it = *first;
				++it;
			} else {
				insert(cend(), *first);
			}
			++first;
		}
		erase(it, cend());
	}

	void assignReuseMem(size_type count, const value_type& value) {
		iterator it = begin();
		while (count-- > 0) {
			if (it != end()) {
				*it = value;
				++it;
			} else {
				insert(cend(), value);
			}
		}
		erase(it, cend());
	}

/*
Modification */

	iterator insertDefault(const_iterator pos, size_type count) {
		iterator it {pos.base()};
		while (count-- > 0) {
			NodeBase* node {newDefaultNode()};
			it = insertNode(it, node);
		}
		return it;
	}

	iterator insertNode(const_iterator pos, NodeBase* node) {
		NodeBase* next {pos.base()};
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

	void insertRange(const_iterator pos, const_iterator first, const_iterator last, std::size_t distance) {
		NodeBase* next {pos.base()};
		first.base()->prev = next->prev;
		last.base()->next = next;
		next->prev->next = first.base();
		next->prev = last.base();
		if (pos == begin()) {
			start = first.base();
		}
		_size += distance;
	}

/*
Operations */

	void spliceDelta(const_iterator pos, list& other,
					const_iterator first, const_iterator last,
					ptrdiff_t delta) {
		auto prev {std::prev(last)};
		other.unlinkRange(first, last);
		other._size -= delta;
		insertRange(pos, first, prev, delta);
	}

	/*
	O(n log n)
	Each height divides the list size by 2 and multiplies the number of nodes by 2.
	Due to std::advance, each node has does O(n) work.
	The lowest 'level' has n nodes, with an input of list size 1
	Then the next lowest level has n / 2 nodes, with an input of list size 2
	So we get: (1 * n) + (2 * (n / 2)) + (4 * (n / 4)) ... + (n * 1)
	for each level, so the complexity is O(n * h), where h is the height of the tree.
	Which is equal to the amount of times n can be divided by 2 to get to 1 (log n)
	= O(n log n)*/
	template <typename Compare>
	void mergesort(list& lst, Compare comp) {
		if (lst.size() <= 1) {
			return;
		}
		const_iterator midpoint = lst.begin();
		// linear in current iteration's list size
		std::advance(midpoint, lst.size() / 2);
		list rightside;
		rightside.spliceDelta(rightside.end(), lst, midpoint, lst.end(), lst.size() / 2 + lst.size() % 2);
		mergesort(lst, comp);
		mergesort(rightside, comp);
		lst.merge(rightside);
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

	void erase(std::size_t n) noexcept {
		while (n-- > 0) {
			pop_back();
		}
	}

	iterator unlink(const_iterator pos) {
		return unlinkRange(pos, std::next(pos));
	}

	/*
	Unlinks in the range [first, last)
	Warning: does NOT decrement size */
	iterator unlinkRange(const_iterator first, const_iterator last) {
		NodeBase* next {last.base()};
		NodeBase* prev {std::prev(first).base()};
		next->prev = prev;
		prev->next = next;
		if (first == begin()) {
			start = next;
		}
		return iterator(next);
	}

	void destroySentinel() noexcept {
		destroyBaseNode(end());
	}

	void destroyBaseNode(iterator it) noexcept {
		nodebase_allocator.destroy(it.base());
		nodebase_allocator.deallocate(it.base(), 1);
	}

	void destroyNode(iterator it) noexcept {
		Node* ptr {static_cast<Node*>(it.base())};
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
