#include "Zombie.h"
#include "Human.h"
#include <myGameEngine/ResourceManager.h>

void Zombie::init(float speed, glm::vec2 pos) {
	_speed = speed;
	_position = pos;
	_color = myGameEngine::ColorRGBA8(255, 255, 255, 255);
	_health = 150;
	m_textureID = myGameEngine::ResourceManager::getTexture("Textures/zombie.png").id;
}

void Zombie::update(const std::vector<std::string>& levelData,
					std::vector<Human*>& humans, 
					std::vector<Zombie*>& zombies,
					float deltaTime) {
	Human* closestHuman = getNearestHuman(humans);

	if (closestHuman != nullptr) {
		m_direction = glm::normalize(closestHuman->getPosition() - _position);
		_position += m_direction * _speed * deltaTime;

	}

	collideWithLevel(levelData);
}

Human* Zombie::getNearestHuman(std::vector<Human*>& humans) {

	Human* closestHuman = nullptr;
	float smallestDistance = 999999.0f;

	for (size_t i = 0; i < humans.size(); ++i) {
		glm::vec2 distVec = humans[i]->getPosition() - _position;
		float distance = glm::length(distVec);

		if (distance < smallestDistance) {
			smallestDistance = distance;
			closestHuman = humans[i];
		}
	}

	return closestHuman;

}