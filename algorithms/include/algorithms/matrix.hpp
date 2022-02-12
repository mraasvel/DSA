#pragma once

#include <cinttypes>
#include <vector>
#include <cassert>
#include <iostream>

namespace DSA {

template <typename T>
class Matrix {
public:
	using size_type = std::size_t;
	using value_type = T;
public:
	Matrix(size_type rows, size_type columns)
	: height(rows), width(columns), map(rows * columns) {}
	~Matrix() {}

	/*
	O(n^3) */
	Matrix operator*(const Matrix& b) const {
		assert(width == b.height && height == b.width);
		Matrix c {height, b.width};
		for (size_type y = 0; y < height; ++y) {
			for (size_type x = 0; x < b.width; ++x) {
				for (size_type k = 0; k < width; ++k) {
					c.get(y, x) += (get(y, k) * b.get(k, x));
				}
			}
		}
		return c;
	}

	T& get(size_type y, size_type x) {
		return map[computeIndex(y, x)];
	}

	const T& get(size_type y, size_type x) const {
		return map[computeIndex(y, x)];
	}

	size_type getWidth() const {
		return width;
	}

	size_type getHeight() const {
		return height;
	}

	template <typename U>
	friend std::ostream& operator<<(std::ostream& out, const Matrix<U>& m);

private:
	size_type computeIndex(size_type y, size_type x) const {
		return y * width + x;
	}

private:
	size_type height;
	size_type width;
	std::vector<T> map;
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const DSA::Matrix<T>& m) {
	for (std::size_t y = 0; y < m.height; ++y) {
		for (std::size_t x = 0; x < m.width; ++x) {
			out << m.get(y, x) << ' ';
		}
		out << std::endl;
	}
	return out;
}

}
