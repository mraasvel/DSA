#pragma once

#include "list/list.hpp"
#include <cassert>
#include <iostream>

template <typename T, typename Alloc>
bool isValidList(const DS::list<T, Alloc>& list) {
	for (auto it = list.begin(); it != list.end(); ++it) {
		if (it.base()->prev->next != it.base()
			|| it.base()->next->prev != it.base()) {
			return false;
		}
	}
	if (list.size() > 0) {
		if (list.end().base()->prev->next != list.end().base()
			|| list.end().base()->next->prev != list.end().base()) {
			return false;
		}
	}
	return true;
}
