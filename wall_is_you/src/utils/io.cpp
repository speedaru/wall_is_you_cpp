#include "pch.h"
#include "io.hpp"
#include <fstream>
#include <iostream>


std::string sp::utils::io::ReadFileStr(const fs::path& path) {
	std::ifstream file(path, std::ios::ate);
	if (!file.is_open()) {
		fprintf(stderr, "failed to open %s\n", path.string().c_str());
		return std::string();
	}

	size_t fileSize = file.tellg();
	file.seekg(std::ios::beg);

	std::string data(fileSize, '\0');
	file.read(data.data(), fileSize);

	return data;
}

