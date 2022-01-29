#include "algorithms/algorithms.hpp"
#include "algorithms/maximum_subarray.hpp"
#include "timer.hpp"
#include "util.hpp"
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>
#include <chrono>

template <typename C>
void printC(const C& c) {
	std::cout << "Container:";
	for (const auto& x : c) {
		std::cout << ' ' << x;
	}
	std::cout << std::endl;
}

std::vector<int> randomVector(size_t n, int max = std::numeric_limits<int>::max()) {
	std::vector<int> v;
	while (n-- > 0) {
		v.push_back(util::randomRangeMersenne(1, max));
	}
	return v;
}

template <typename Function, typename... Args>
double Benchmark(bool coldrun, Function func, Args&&... args) {
	util::Timer timer;
	if (coldrun) {
		func(std::forward<Args>(args)...);
	}
	timer.reset();
	func(std::forward<Args>(args)...);
	return timer.elapsed();
}

void benchmarkMaxSubarray(size_t n) {
	std::vector<int> v {randomVector(n)};
	auto time = Benchmark(true, &DSA::maximumSubarray<std::vector<int>::const_iterator>, v.begin(), v.end());
	auto timebf = Benchmark(true, &DSA::maximumSubarrayBF<std::vector<int>::const_iterator>, v.begin(), v.end());
	auto timeln = Benchmark(true, &DSA::maximumSubarrayLinear<std::vector<int>::const_iterator>, v.begin(), v.end());
	std::cout << __FUNCTION__ << ": " << n << std::endl;
	std::cout << "  NM: " << std::fixed << time << std::endl;
	std::cout << "  BF: " << std::fixed << timebf << std::endl;
	std::cout << "  LN: " << std::fixed << timeln << std::endl;
}

int main() {
	srand(time(0));

	benchmarkMaxSubarray(18);
	benchmarkMaxSubarray(42);
	benchmarkMaxSubarray(1000);
	benchmarkMaxSubarray(10000);
	return 0;
}
