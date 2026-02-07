#pragma once
#include "pch.h"


namespace sp {
	namespace utils {
		// dir = -1 trim left, dir = 0 both, dir = 1 trim right
		std::string TrimString(const std::string& str, const std::string& sub, int dir);
	}
}
