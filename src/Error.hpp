#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>

namespace pberr {
	std::string str;
	inline void push(const char *s) {
		str += s;
		str += '\n';
	}
	inline void clear() {
		str.clear();
	}
	inline bool empty() {
		return str.empty();
	}
}

#endif
