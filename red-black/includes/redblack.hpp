#ifndef REDBLACK_HPP
# define REDBLACK_HPP

# include "redblack_node.hpp"
# include "redblack_iterator.hpp"

# include <utility>
# include <cstddef>
# include <functional>
# include <memory>

template <
		typename T,
		typename Compare = std::less<T>,
		typename Alloc = std::allocator<T> >
class RedBlackTree {
public:
	typedef T	value_type;
	typedef Compare compare_type;
	typedef Alloc allocator_type;

	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef RedBlackNode<value_type> node_type;
	typedef typename node_type::const_node_type const_node_type;
	typedef typename node_type::node_pointer node_pointer;
	typedef typename node_type::const_node_pointer const_node_pointer;

	typedef typename allocator_type::template rebind<node_type>::other node_allocator_type;

	typedef RedBlackIterator<T> iterator;
	typedef RedBlackIterator<const T> const_iterator;

public:

	RedBlackTree(const compare_type &comp = compare_type())
	: root(NULL), compare(comp), _size(0), min_node(NULL), max_node(NULL) {
		initializeEnd();
	}

	RedBlackTree(const RedBlackTree& from)
	: root(NULL), compare(from.compare), alloc(from.alloc), _size(0), min_node(NULL), max_node(NULL) {
		initializeEnd();
		*this = from;
	}

	~RedBlackTree() {
		clear();
		destroyNode(end_node);
	}
	RedBlackTree& operator=(const RedBlackTree& rhs) {
		if (this == &rhs) {
			return *this;
		}
		assign(rhs);
		return *this;
	}

/* Iterators */
	iterator begin() {
		return createIterator(min_node);
	}

	const_iterator begin() const {
		return createIterator(min_node);
	}

	iterator end() {
		return createIterator(end_node);
	}

	const_iterator end() const {
		return createIterator(end_node);
	}

/* Capacity */

	size_type size() const {
		return _size;
	}

	size_type max_size() const {
		return alloc.max_size();
	}

	bool empty() const {
		return size() == 0;
	}

/* Modifiers */

	iterator insert(const value_type& v) {
		insertValue(v);
		checkMinMax();
		updateEnd();
		return createIterator(last_node);
	}

	void erase(const value_type& v) {
		erase(find(v));
	}

	void erase(iterator position) {
		node_pointer x = position.base();
		if (!isNull(x)) {
			eraseNode(x);
		}

		if (size() == 0) {
			zeroTree();
		} else {
			root->turnBlack();
		}
		updateEnd();
	}

	void swap(RedBlackTree& rhs) {
		std::swap(root, rhs.root);
		std::swap(min_node, rhs.min_node);
		std::swap(max_node, rhs.max_node);
		std::swap(end_node, rhs.end_node);
		std::swap(_size, rhs._size);
	}

	void clear() {
		destroyTree(base());
		zeroTree();
	}

/* Observers */

	compare_type value_comp() const {
		return compare;
	}

/* Operations */

	iterator find(const value_type& v) {
		return createIterator(findNode(v, base()));
	}

	const_iterator find(const value_type& v) const {
		return createIterator(findNode(v, base()));
	}

	size_type count(const value_type& val) const {
		return find(val) != end() ? 1 : 0;
	}

	iterator lower_bound(const value_type& k) {
		return lowerBoundInternal(k);
	}

	const_iterator lower_bound(const value_type& k) const {
		return lowerBoundInternal(k);
	}

	iterator upper_bound(const value_type& k) {
		iterator it = lower_bound(k);
		if (equalValue(*it, k)) {
			++it;
		}
	
		return it;
	}

	const_iterator upper_bound(const value_type& k) const {
		const_iterator it = lower_bound(k);
		if (equalValue(*it, k)) {
			++it;
		}
	
		return it;
	}

	std::pair<iterator, iterator> equal_range(const value_type& val) {
		iterator itlow = lower_bound(val);
		iterator itup(itlow);
		if (equalValue(*itup, val)) {
			++itup;
		}
		return std::make_pair(itlow, itup);
	}

	std::pair<const_iterator, const_iterator> equal_range(const value_type& val) const {
		const_iterator itlow = lower_bound(val);
		const_iterator itup(itlow);
		if (equalValue(*itup, val)) {
			++itup;
		}
		return std::make_pair(itlow, itup);
	}


public:
	node_pointer base() {
		return root;
	}

	node_pointer base() const {
		return root;
	}

private:
	bool isNull(const node_pointer x) const {
		return x == NULL;
	}

	bool equalValue(const value_type& a, const value_type& b) const {
		return !(compare(a, b) || compare(b, a));
	}

	bool compareNodes(const node_pointer a, const node_pointer b) const {
		return compare(a->getValue(), b->getValue());
	}

	void checkMinMax() {
		if (min_node == NULL || compareNodes(last_node, min_node)) {
			min_node = last_node;
		}

		if (max_node == NULL || compareNodes(max_node, last_node)) {
			max_node = last_node;
		}
	}

	void zeroTree() {
		root = NULL;
		_size = 0;
		max_node = NULL;
		min_node = NULL;
	}

	const_node_pointer getConstNode(node_pointer x) const {
		return reinterpret_cast<const_node_pointer> (x);
	}

	iterator createIterator(node_pointer x) {
		if (isNull(x) || x == end_node) {
			return iterator(NULL, endNode(), true);
		}

		return iterator(x, endNode(), false);
	}

	const_iterator createIterator(node_pointer x) const {
		if (isNull(x) || x == end_node) {
			return const_iterator(NULL, endNode(), true);
		}

		return const_iterator(getConstNode(x), endNode(), false);
	}

/* Node Allocation */
	node_pointer newNode(const value_type& a) {
		return newNode(a, NULL);
	}

	node_pointer newNode(const value_type& a, node_pointer parent) {
		last_node = alloc.allocate(1);
		try {
			alloc.construct(last_node, node_type(a, parent));	
		} catch (...) {
			alloc.deallocate(last_node, 1);
			throw;
		}
		return last_node;
	}

	void destroyNodeCheckMinMax(node_pointer x) {
		if (size() != 0) {
			if (x == min_node) {
				min_node = findMin(root);
			} else if (x == max_node) {
				max_node = findMax(root);
			}
		}
		destroyNode(x);
	}

	void destroyNode(node_pointer x) {
		alloc.destroy(x);
		alloc.deallocate(x, 1);
	}

/* Node Getters */
	bool isBlack(node_pointer x) {
		return isNull(x) || x->isBlack();
	}

	bool isRed(node_pointer x) {
		return !isNull(x) && x->isRed();
	}

/* Rotations */

	void rotateLeft(node_pointer x) {
		node_pointer y = x->right;

		replaceParentLink(x, y);
		y->parent = x->parent;
		x->right = y->left;
		x->parent = y;
		if (!isNull(x->right)) {
			x->right->parent = x;
		}
		y->left = x;
	}

	void rotateRight(node_pointer x) {
		node_pointer y = x->left;
		replaceParentLink(x, y);
		y->parent = x->parent;
		x->left = y->right;
		x->parent = y;
		if (!isNull(x->left)) {
			x->left->parent = x;
		}
		y->right = x;
	}

	void replaceParentLink(node_pointer x, node_pointer replacement) {
		if (isNull(x->parent)) {
			root = replacement;
		} else if (x == x->parent->right) {
			x->parent->right = replacement;
		} else {
			x->parent->left = replacement;
		}
	}

/* Insertion */

	void insertValue(const value_type& v) {

		node_pointer x = findInsertPosition(v);
		if (isNull(x)) {
			root = newNode(v);
		} else if (compare(v, x->getValue())) {
			x->left = newNode(v, x);
		} else if (compare(x->getValue(), v)) {
			x->right = newNode(v, x);
		} else {
			last_node = x;
			return;
		}

		insertFixProperty(last_node);
		_size += 1;
	}

	/*
	** Return: node with value v if exists, or parent of insert position
	*/
	node_pointer findInsertPosition(const value_type& v) const {

		if (isNewMin(v)) {
			return min_node;
		} else if (isNewMax(v)) {
			return max_node;
		}

		node_pointer x = base();
		node_pointer parent = NULL;
		while (!isNull(x)) {
			parent = x;
			if (compare(v, x->getValue())) {
				x = x->left;
			} else if (compare(x->getValue(), v)) {
				x = x->right;
			} else {
				return x;
			}
		}
		return parent;
	}

	bool isNewMin(const value_type& v) const {
		return isNull(min_node) || compare(v, min_node->getValue());
	}

	bool isNewMax(const value_type& v) const {
		return isNull(max_node) || compare(max_node->getValue(), v);
	}

	/*
	** https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Insertion
	** https://www.codesdope.com/course/data-structures-red-black-trees-insertion/
	*/

	void insertFixProperty(node_pointer x) {

		while (isRed(x->parent)) {
			node_pointer uncle = x->getUncle();
			if (isRed(uncle)) {
				insertCaseOne(x, uncle);
				x = x->parent->parent;
			} else {
				x = insertCaseTwo(x);
				insertCaseThree(x);
			}
		}
		root->turnBlack();
	}

	void insertCaseOne(node_pointer x, node_pointer uncle) {
		x->parent->turnBlack();
		uncle->turnBlack();
		uncle->parent->turnRed();
	}

	node_pointer insertCaseTwo(node_pointer x) {
		node_pointer y = x->parent;
		if (x == x->parent->right && x->parent == x->parent->parent->left) {
			rotateLeft(x->parent);
		} else if (x == x->parent->left && x->parent == x->parent->parent->right) {
			rotateRight(x->parent);
		} else {
			return x;
		}

		return y;
	}

	void insertCaseThree(node_pointer x) {
		x->parent->turnBlack();
		x->parent->parent->turnRed();
		if (x->parent == x->parent->parent->left) {
			rotateRight(x->parent->parent);
		} else {
			rotateLeft(x->parent->parent);
		}
	}

/* Node Deletion */

	void eraseNode(node_pointer x) {
		if (isNull(x->left)) {
			cutNodeLink(x, x->right);
			// if X is black and it's children are both NULL, then there's a property 4 violation
			// Possibilities: x->right is NULL or x->right is RED
			if (isRed(x->right)) {
				x->right->turnBlack();
			} else if (isBlack(x) && !isNull(x->parent)) {
				eraseFixProperty(x->right, x->parent);
			}
		} else if (isNull(x->right)) {
			cutNodeLink(x, x->left);
			// X has to be black, because it cannot have one child as a red node
			// Meaning that x->left has to be red in this case
			// So we turn it black to preserve property 4
			x->left->turnBlack();
		} else {
			node_pointer y = findMin(x->left);
			swapNode(x, y);
			eraseNode(x);
			return;
		}
		_size -= 1;
		destroyNodeCheckMinMax(x);
	}

	void cutNodeLink(node_pointer x, node_pointer y) {
		if (!isNull(y)) {
			y->parent = x->parent;
		}
		if (isNull(x->parent)) {
			root = y;
			return;
		}
		if (x == x->parent->right) {
			x->parent->right = y;
		} else {
			x->parent->left = y;
		}
	}

	/*
	** https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Removal:_simple_cases
	** https://www.codesdope.com/course/data-structures-red-black-trees-deletion/
	*/

	void eraseFixProperty(node_pointer x, node_pointer parent) {
		do {
			node_pointer sibling = (parent->left == x) ? parent->right : parent->left;

			if (eraseCaseOne(parent, sibling)) {
				sibling->turnRed();
				x = parent;
			} else {
				if (isRed(sibling)) {
					sibling = eraseCaseThree(x, parent, sibling);
				}
				// Case 4, 5, and 6 are now checked
				eraseFixFinalize(parent, sibling);
				return;
			}
			parent = x->parent;
		} while (!isNull(parent));
	}

	bool eraseCaseOne(node_pointer parent, node_pointer sibling) {
		return	isBlack(parent) &&
				isBlack(sibling) &&
				isBlack(sibling->left) &&
				isBlack(sibling->right);
	}

	node_pointer eraseCaseThree(node_pointer x, node_pointer parent, node_pointer sibling) {
		parent->turnRed();
		sibling->turnBlack();
		if (x == parent->left) {
			rotateLeft(parent);
			return parent->right;
		} else {
			rotateRight(parent);
			return parent->left;
		}
	}

	void eraseFixFinalize(node_pointer parent, node_pointer sibling) {
		if (eraseCaseFour(parent, sibling)) {
			sibling->turnRed();
			parent->turnBlack();
			return;
		}

		node_pointer close_nephew = (sibling == parent->left) ? sibling->right : sibling->left;
		node_pointer distant_nephew = (sibling == parent->left) ? sibling->left : sibling->right;

		if (eraseCaseFive(sibling, close_nephew, distant_nephew)) {
			if (sibling == parent->right) {
				rotateRight(sibling);
			} else {
				rotateLeft(sibling);
			}
			sibling->turnRed();
			close_nephew->turnBlack();
			distant_nephew = sibling;
			sibling = close_nephew;
		}

		eraseCaseSix(parent, sibling, distant_nephew);
	}

	bool eraseCaseFour(node_pointer parent, node_pointer sibling) {
		return	isRed(parent) &&
				isBlack(sibling) &&
				isBlack(sibling->left) &&
				isBlack(sibling->right);
	}

	bool eraseCaseFive(node_pointer sibling, node_pointer close_nephew, node_pointer distant_nephew) {
		return	isBlack(sibling) &&
				isRed(close_nephew) &&
				isBlack(distant_nephew);
	}

	void eraseCaseSix(node_pointer parent, node_pointer sibling, node_pointer distant_nephew) {
		if (!(isBlack(sibling) && isRed(distant_nephew))) {
			return;
		}
		if (sibling == parent->right) {
			rotateLeft(parent);
		} else {
			rotateRight(parent);
		}
		sibling->assignColor(parent->getColor());
		parent->turnBlack();
		distant_nephew->turnBlack();
	}

/* Node Swapping */

	void swapNode(node_pointer x, node_pointer y) {
		if (x->parent == y) {
			swapNode(y, x);
		}
		replaceParentPointer(x, y);
		replaceParentPointer(y, x);
		swapPointers(x, y);
		if (x->parent == x) {
			x->parent = y;
		}
		updateChildPointers(x);
		updateChildPointers(y);
	}

	void replaceParentPointer(node_pointer x, node_pointer y) {
		if (isNull(x->parent)) {
			root = y;
		} else if (x == x->parent->left) {
			x->parent->left = y;
		} else {
			x->parent->right = y;
		}
	}

	void swapPointers(node_pointer x, node_pointer y) const {
		std::swap(x->left, y->left);
		std::swap(x->right, y->right);
		std::swap(x->parent, y->parent);
		std::swap(x->getColor(), y->getColor());
	}

	void updateChildPointers(node_pointer x) const {
		if (!isNull(x->left)) {
			x->left->parent = x;
		}

		if (!isNull(x->right)) {
			x->right->parent = x;
		}
	}

/* Searching */

	node_pointer findNode(const value_type& v, node_pointer x) const {
		while (!isNull(x)) {
			if (compare(v, x->getValue())) {
				x = x->left;
			} else if (compare(x->getValue(), v)) {
				x = x->right;
			} else {
				return x;
			}
		}
		return NULL;
	}

	node_pointer findMin(node_pointer x) const {
		if (isNull(x)) {
			return NULL;
		}

		while (!isNull(x->left)) {
			x = x->left;
		}
		return x;
	}

	node_pointer findMax(node_pointer x) const {
		if (isNull(x)) {
			return NULL;
		}
		while (!isNull(x->right)) {
			x = x->right;
		}
		return x;
	}

/* Deep Copy, Assignation */
	void assign(const RedBlackTree& x) {
		clear();
		root = copyTree(x.base(), NULL, x);
		_size = x.size();
		updateEnd();
	}

	node_pointer copyTree(node_pointer x, node_pointer parent, const RedBlackTree& from) {
		if (isNull(x)) {
			return NULL;
		}
		node_pointer y = copyNode(x, parent);
		y->left = copyTree(x->left, y, from);
		y->right = copyTree(x->right, y, from);
		if (x == from.max_node) {
			max_node = x;
		}
		if (x == from.min_node) {
			min_node = x;
		}
		return y;
	}

	node_pointer copyNode(node_pointer x, node_pointer parent) {
		node_pointer y = alloc.allocate(1);
		try {
			alloc.construct(y, *x);
		} catch (...) {
			alloc.deallocate(y, 1);
			throw;
		}
		y->parent = parent;
		return y;
	}

/* End Node */
	void initializeEnd() {
		end_node = alloc.allocate(1);
		try {
			alloc.construct(end_node, node_type());
		} catch (...) {
			alloc.deallocate(end_node, 1);
			throw;
		}
	}

	void updateEnd() {
		end_node->left = max_node;
	}

	node_pointer endNode() {
		return end_node;
	}

	const_node_pointer endNode() const {
		return getConstNode(end_node);
	}

/* Destruction */
	void destroyTree(node_pointer x) {
		if (isNull(x)) {
			return;
		}

		destroyTree(x->left);
		destroyTree(x->right);
		destroyNode(x);
	}

/* Operations */

	iterator lowerBoundInternal(const value_type& k) {
		node_pointer x = base();

		while (!isNull(x)) {
			if (compare(k, x->getValue())) {
				if (isNull(x->left)) {
					return createIterator(x);
				}
				x = x->left;
			} else if (compare(x->getValue(), k)) {
				if (isNull(x->right)) {
					x = x->nextNode();
					return createIterator(x);
				}
				x = x->right;
			} else {
				return createIterator(x);
			}
		}

		return end();
	}

	const_iterator lowerBoundInternal(const value_type& k) const {
		node_pointer x = base();

		while (!isNull(x)) {
			if (compare(k, x->getValue())) {
				if (isNull(x->left)) {
					return createIterator(x);
				}
				x = x->left;
			} else if (compare(x->getValue(), k)) {
				if (isNull(x->right)) {
					x = x->nextNode();
					return createIterator(x);
				}
				x = x->right;
			} else {
				return createIterator(x);
			}
		}

		return end();
	}

private:
	node_pointer root;
	compare_type compare;
	node_allocator_type alloc;
	size_type _size;

	node_pointer last_node;
	node_pointer min_node;
	node_pointer max_node;
	node_pointer end_node;
};

template< class Key, class Compare, class Alloc >
bool operator==(const RedBlackTree<Key, Compare, Alloc>& lhs,
				const RedBlackTree<Key, Compare, Alloc>& rhs ) {
	if (lhs.size() != rhs.size()) {
		return false;
	}

	return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
template< class Key, class Compare, class Alloc >
bool operator!=(const RedBlackTree<Key, Compare, Alloc>& lhs,
				const RedBlackTree<Key, Compare, Alloc>& rhs) {
	return !(lhs == rhs);
}
template< class Key, class Compare, class Alloc >
bool operator<( const RedBlackTree<Key, Compare, Alloc>& lhs,
				const RedBlackTree<Key, Compare, Alloc>& rhs) {
	return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
template< class Key, class Compare, class Alloc >
bool operator<=(const RedBlackTree<Key, Compare, Alloc>& lhs,
				const RedBlackTree<Key, Compare, Alloc>& rhs) {
	return !(rhs < lhs);
}
template< class Key, class Compare, class Alloc >
bool operator>( const RedBlackTree<Key, Compare, Alloc>& lhs,
				const RedBlackTree<Key, Compare, Alloc>& rhs) {
	return rhs < lhs;
}
template< class Key, class Compare, class Alloc >
bool operator>=(const RedBlackTree<Key, Compare, Alloc>& lhs,
				const RedBlackTree<Key, Compare, Alloc>& rhs) {
	return !(lhs < rhs);
}

#endif /* REDBLACK_HPP */
