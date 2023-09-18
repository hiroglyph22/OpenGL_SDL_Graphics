#pragma once

#include <vector>
#include <string>

namespace myGameEngine {
	class IOManager
	{
	public:
		static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);

	};
}
