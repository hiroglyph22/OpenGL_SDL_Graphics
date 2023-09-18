#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

namespace myGameEngine {

class InputManager
{
public:
	InputManager();

	void update();

	void pressKey(unsigned int keyID);
	void releaseKey(unsigned int keyID);

	// Returns true if the key is held down
	bool isKeyDown(unsigned int keyID);
	// Returns true if the key was just pressed
	bool isKeyPressed(unsigned int keyID);
	void setMouseCoords(float x, float y);

	// putting const says that this function won't change anything within
	// InputManager
	// getters
	glm::vec2 getMouseCoords() const { return _mouseCoords; }
private:

	bool wasKeyDown(unsigned int keyID);

	std::unordered_map<unsigned int, bool> _keyMap;
	std::unordered_map<unsigned int, bool> _previousKeyMap;
	glm::vec2 _mouseCoords;
};

}

