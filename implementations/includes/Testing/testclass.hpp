#pragma once

#include <utility>
#include <iostream>

namespace Testing {

template <typename T>
class TestClass {
public:
	TestClass() {
		std::cout << "Default Constructor" << std::endl;
	}

	TestClass(const TestClass& rhs)
	: value(rhs.value) {
		std::cout << "Copy Constructor" << std::endl;
	}

	TestClass(TestClass&& rhs) {
		std::cout << "Move Constructor" << std::endl;
		std::swap(value, rhs.value);
	}

	TestClass& operator=(const TestClass& rhs) {
		std::cout << "Copy Assignment" << std::endl;
		value = rhs.value;
		return *this;
	}

	TestClass& operator=(TestClass&& rhs) {
		std::cout << "Move Assignment" << std::endl;
		std::swap(value, rhs.value);
	}

	~TestClass() {
		std::cout << "Destructor" << std::endl;
	}

	TestClass(const T& value)
	: value(value) {
		std::cout << "  Value Constructor" << std::endl;
	}

private:
	T value;
};

}
