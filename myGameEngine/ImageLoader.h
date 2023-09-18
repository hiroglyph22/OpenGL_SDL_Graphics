#pragma once

#include "GLTexture.h"
#include <string>

namespace myGameEngine {
	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);

	};
}

