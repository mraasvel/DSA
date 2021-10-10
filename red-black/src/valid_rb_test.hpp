#pragma once

#include "redblack.hpp"
#include "redblack_iterator.hpp"
#include <cassert>

namespace Test {

template <typename Node>
bool isBlack(const Node* root) {
	return root == NULL || root->getColor() == Node::BLACK;
}

template <typename Node>
int blackHeight(const Node* root) {
	if (root == NULL || (root->left && root->left->right == root)) {
		return 0;
	}

	int blackHeightLeft = blackHeight(root->left);
	int blackHeightRight = blackHeight(root->right);
	assert(blackHeightLeft == blackHeightRight);

	if (isBlack(root)) {
		return blackHeightLeft + 1;
	}

	return blackHeightLeft;
}

template <typename Node>
int testNode(const Node* x) {
	if (x == NULL || (x->left && x->left->right == x)) {
		return 0;
	}

	if (x->right) {
		assert(x->right->parent == x);
		assert(x->right->getValue() > x->getValue());
	}

	if (x->left) {
		assert(x->left->parent == x);
		assert(x->left->getValue() < x->getValue());
	}

	if (x->getColor() == Node::RED) {
		assert(x->parent->getColor() == Node::BLACK);
	}

	assert(testNode(x->left) == testNode(x->right));
	return blackHeight(x);
}

template <typename Node>
bool testRedBlackInvariant(const Node* root) {
	if (root == NULL) {
		return true;
	}

	assert(root->getColor() == Node::BLACK);
	testNode(root);
	return true;
}

template <typename T>
bool validRB(const RedBlackTree<T>& m) {
	return testRedBlackInvariant(m.base());
}

}
