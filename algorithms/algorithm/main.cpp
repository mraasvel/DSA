#include "algorithms/algorithms.hpp"
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>

template <typename C>
void printC(const C& c) {
	std::cout << "Container:";
	for (const auto& x : c) {
		std::cout << ' ' << x;
	}
	std::cout << std::endl;
}

int main() {
	std::vector<int> v {5, 2, 4, 6, 1, 3};
	DSA::insertionSort(v.begin(), v.end());
	std::cout << "5! = " << DSA::Factorial(5) << std::endl;
	std::cout << "Is sorted: " << std::boolalpha << std::is_sorted(v.begin(), v.end()) << std::endl;
	printC(v);
	DSA::insertionSort(v.begin(), v.end(), std::greater<int>());
	printC(v);
	std::cout << "Is sorted: " << std::boolalpha << std::is_sorted(v.begin(), v.end(), std::greater<int>()) << std::endl;
	return 0;
}
