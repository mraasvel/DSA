#ifndef UTIL_HPP
#define UTIL_HPP

#include <vector>
#include <string>

namespace util {

/*
Directory, filesystem
*/
bool isDirectory(const std::string& dirname);
std::vector<std::string> directorySearch(const std::string& dirname);

/*
String manipulation
*/
std::vector<std::string> stringSplit(const std::string& input, const std::string& delim_set);
std::vector<std::string> stringSplit(const std::string& input, char delim);

std::string itoa(unsigned long n, unsigned int base = 10);
std::string itoa(unsigned int n, unsigned int base = 10);
std::string itoa(long n, unsigned int base = 10);
std::string itoa(int n, unsigned int base = 10);

/*
x -- string to search
to_find -- substring to search for
limit -- max characters to traverse in x
*/
std::size_t findLimit(const std::string& x, const std::string& to_find, std::size_t limit);

/*
Random Number Generation
	Prefer: randomRangeMersenne
*/
double randomFraction();
int randomRange(int min, int max);
int randomRangeMersenne(int min, int max);

}

#endif /* UTIL_HPP */
