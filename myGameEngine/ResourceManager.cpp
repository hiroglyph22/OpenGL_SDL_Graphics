#include "ResourceManager.h"

namespace myGameEngine {

	TextureCache ResourceManager::_textureCache;

	// Wrapper function
	GLTexture ResourceManager::getTexture(std::string texturePath) {
		return _textureCache.getTexture(texturePath);
	}

}