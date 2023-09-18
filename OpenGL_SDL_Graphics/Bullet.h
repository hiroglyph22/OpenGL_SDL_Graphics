#pragma once

#include <glm/glm.hpp>
#include <myGameEngine/SpriteBatch.h>

class Bullet
{
public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime);

	void draw(myGameEngine::SpriteBatch& spriteBatch);
	// Returns true when the bullet is out of life
	bool update();
private:
	float _speed;
	int _lifeTime;
	glm::vec2 _direction;
	glm::vec2 _position;
};

