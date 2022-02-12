#include "algorithms/matrix.hpp"
#include <catch2/catch.hpp>

template <typename T>
void fillMatrix(DSA::Matrix<T>& m) {
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			m.get(i, j) = i * m.getWidth() + j;
		}
	}
}

TEST_CASE("matrix", "[matrix]") {
	DSA::Matrix<int> m {5, 5};
	DSA::Matrix<int> m2 {5, 5};
	fillMatrix(m);
	fillMatrix(m2);
	std::cout << m << std::endl;
	std::cout << m2 << std::endl;
	std::cout << (m * m2) << std::endl;
}
