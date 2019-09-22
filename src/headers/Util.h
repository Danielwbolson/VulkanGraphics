#pragma once

#include <vector>
#include <fstream>
#include <iostream>

namespace util {
	 
	static std::vector<char> readFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		// std::ios::ate signifies starting reading at end of file
		// useful for allocating buffer

		if (!file.is_open()) {
			throw std::runtime_error("Failed to open file");
		}

		size_t filesize = (size_t)file.tellg();
		std::vector<char> buffer(filesize);

		file.seekg(0);
		file.read(buffer.data(), filesize);

		file.close();

		return buffer;
	}

}