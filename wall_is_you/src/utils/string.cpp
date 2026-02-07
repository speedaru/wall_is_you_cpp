#include "pch.h"
#include "string.hpp"


std::string sp::utils::TrimString(const std::string& str, const std::string& sub, int dir) {
	std::string trimmed(str);
	size_t subSize = sub.size();

	// trim left
	if (dir != 1) {
		while (trimmed.find(sub) == 0) { // while found at begining
			trimmed = trimmed.erase(0, subSize);
		}
	}
	
	// trim right
	if (dir != -1) {
		size_t idx = 0ull;
		while (trimmed.size() >= subSize && (idx = trimmed.find(sub)) == trimmed.size() - subSize) {
			trimmed = trimmed.erase(idx, subSize);
		}
	}

	return trimmed;
}
