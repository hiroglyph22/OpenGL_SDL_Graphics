#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <myGameEngine/AudioEngine.h>

#include "Bullet.h"

class Gun
{
public:
	Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletDamage,
		float bulletSpeed, myGameEngine::SoundEffect fireEffect);

	void update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, 
		std::vector<Bullet>& bullets, float deltaTime);
	
private:
	void fire(const glm::vec2& direction, const glm::vec2& position, 
		std::vector<Bullet>& bullets);

	myGameEngine::SoundEffect m_fireEffect;
	std::string _name;
	int _fireRate;
	int _bulletsPerShot;
	float _spread;
	float _bulletSpeed;
	int _bulletDamage;
	float _frameCounter;
};

