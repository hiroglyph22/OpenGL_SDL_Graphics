#pragma once

#include "Human.h"
#include <myGameEngine/InputManager.h>
#include <myGameEngine/Camera2D.h>
#include "Bullet.h"

class Gun;

class Player : public Human
{
public:
	Player();
	
	void init(float speed, glm::vec2 pos, myGameEngine::InputManager* inputManager,
		myGameEngine::Camera2D* camera, std::vector<Bullet>* bullets);

	void addGun(Gun* gun);

	void update(const std::vector<std::string>& levelData,
					std::vector<Human*>& humans, 
					std::vector<Zombie*>& zombies,
					float deltaTime) override;
private: 
	myGameEngine::InputManager* _inputManager;
	std::vector<Gun*> _guns;
	int _currentGunIndex;
	myGameEngine::Camera2D* _camera;
	std::vector<Bullet>* _bullets;
};

