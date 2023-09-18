#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>

namespace myGameEngine {

	TextureCache::TextureCache() {
	}

	TextureCache::~TextureCache() {
	}

	GLTexture TextureCache::getTexture(std::string texturePath) {

		// Only use auto for super long classes
		// Lookup the texture and see if its in the map
		auto mit = _textureMap.find(texturePath);

		// check if its not in the map
		if (mit == _textureMap.end()) {
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);

			// a pair is 2 values that are put together  
			_textureMap.insert(make_pair(texturePath, newTexture));

			return newTexture;
		}
		// mit->second is the actual texture
		return mit->second;
	}

}