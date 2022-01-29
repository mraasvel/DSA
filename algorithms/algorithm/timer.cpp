#include "timer.hpp"

namespace util {

Timer::Timer()
: start_time(clock_type::now()) {}

void Timer::reset() {
	start_time = clock_type::now();
}

double Timer::elapsed() const {
	return std::chrono::duration_cast<second_type>(clock_type::now() - start_time).count();
}

}
