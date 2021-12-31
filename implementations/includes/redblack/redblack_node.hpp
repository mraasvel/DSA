#ifndef REDBLACK_NODE_HPP
# define REDBLACK_NODE_HPP

# include <cstddef>

template <typename T>
class RedBlackNode {
public:
	typedef T value_type;

	typedef RedBlackNode<T> node_type;
	typedef RedBlackNode<const T> const_node_type;
	typedef RedBlackNode<T>* node_pointer;
	typedef RedBlackNode<const T>* const_node_pointer;

	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;


	enum ColorType {
		RED,
		BLACK
	};

	typedef enum ColorType color_type;

private:
	RedBlackNode(node_pointer left, node_pointer right, node_pointer parent, const value_type& val, color_type color)
	: left(left), right(right), parent(parent), value(val), color(color) {}
public:
	RedBlackNode()
	: left(NULL), right(NULL), parent(NULL), color(RED) {}
	
	RedBlackNode(const RedBlackNode& from)
	: left(from.left), right(from.right), parent(from.parent), value(from.getValue()), color(from.getColor()) {}

	RedBlackNode(const value_type& val)
	: left(NULL), right(NULL), parent(NULL), value(val), color(RED) {}

	RedBlackNode(const value_type& val, node_pointer parent)
	: left(NULL), right(NULL), parent(parent), value(val), color(RED) {}


	~RedBlackNode() {}

	reference getValue() {
		return value;
	}

	const_reference getValue() const {
		return value;
	}

	void assignColor(color_type c) {
		color = c;
	}

	void turnBlack() {
		color = BLACK;
	}

	void turnRed() {
		color = RED;
	}

	bool isBlack() const {
		return color == BLACK;
	}

	bool isRed() const {
		return color == RED;
	}

	color_type& getColor() {
		return color;
	}

	const color_type& getColor() const {
		return color;
	}

	node_pointer getUncle() {
		if (parent == parent->parent->left) {
			return parent->parent->right;
		} else {
			return parent->parent->left;
		}
	}

	node_pointer getSibling() {
		if (this == parent->left) {
			return parent->right;
		} else {
			return parent->left;
		}
	}

	// operator const_node_type() const {
	// 	return const_node_type(left, right, parent, value, color);
	// }

	node_pointer nextNode() const {
		if (right) {
			return findMin(right);
		}

		return nextNodeUp(this, parent);
	}

	node_pointer prevNode() const {
		if (left) {
			return findMax(left);
		}

		return prevNodeUp(this, parent);
	}

private:
	node_pointer findMin(node_pointer x) const {
		while (x->left) {
			x = x->left;
		}

		return x;
	}

	node_pointer findMax(node_pointer x) const {
		while (x->right) {
			x = x->right;
		}

		return x;
	}

	node_pointer nextNodeUp(const RedBlackNode<T>* prev, node_pointer x) const {
		while (x) {
			if (prev == x->left) {
				return x;
			}

			prev = x;
			x = x->parent;
		}
		return NULL;
	}

	node_pointer prevNodeUp(const RedBlackNode<T>* prev, node_pointer x) const {
		while (x) {
			if (prev == x->right) {
				return x;
			}

			prev = x;
			x = x->parent;
		}

		return NULL;
	}

public:
	node_pointer left;
	node_pointer right;
	node_pointer parent;

private:
	value_type value;
	color_type color;
};

#endif /* REDBLACK_NODE_HPP */
