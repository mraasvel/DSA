#include <cstdlib>
#include <ctime>
#include <random>

namespace util {

/*
Return random number in range [0.0, 1.0)
inclusive 0, exclusive 1
*/
double randomFraction() {
	static double fraction = (1.0 / (RAND_MAX + 1.0));
	return static_cast<double>(rand()) * fraction;
}

int randomRange(int min, int max) {
	return min + static_cast<int>((max - min + 1 ) * randomFraction());
}

/*
Using Mersenne

	https://www.learncpp.com/cpp-tutorial/random-number-generation/
	http://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/emt.html

Library:
	https://github.com/effolkronium/random
*/

int randomRangeMersenne(int min, int max) {
	static std::mt19937 mersenne(static_cast<std::mt19937::result_type>(std::time(nullptr)));
	std::uniform_int_distribution<> die(min, max);
	return die(mersenne);
}

}
