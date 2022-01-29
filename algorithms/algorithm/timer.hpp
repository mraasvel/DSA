#ifndef TIMER_HPP
#define TIMER_HPP

/*
NOTE: Min C++11 for chrono
*/

#include <chrono>

namespace util {

class Timer {
private:
	typedef std::chrono::steady_clock clock_type;
	typedef std::chrono::duration<double, std::ratio<1> > second_type;

public:
	Timer();

	void reset();
	double elapsed() const;

private:

	std::chrono::time_point<clock_type> start_time;
};

}

#endif /* TIMER_HPP */
