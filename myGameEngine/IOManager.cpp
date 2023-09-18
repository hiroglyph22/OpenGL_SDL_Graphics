#include "IOManager.h"

#include <fstream>

namespace myGameEngine {

	bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer) {
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail()) {
			perror(filePath.c_str());
			return false;
		}

		// Check how big the file is for the buffer

		// seek to the end
		file.seekg(0, std::ios::end);

		// Get the file size
		int fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		// Take out the header information (probably doesn't matter)
		fileSize -= (int)file.tellg();

		buffer.resize(fileSize);
		file.read((char*)&(buffer[0]), fileSize);
		file.close();

		return true;
	}

}