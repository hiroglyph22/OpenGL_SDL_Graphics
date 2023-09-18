#include "MainGame.h"
#include "Gun.h"
#include "Zombie.h"

#include <SDL/SDL.h>
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include <glm/gtx/rotate_vector.hpp>

#include <myGameEngine/myGameEngine.h>
#include <myGameEngine/Timing.h>
#include <myGameEngine/myGameEngineErrors.h>
#include <myGameEngine/ResourceManager.h>


// Following the Advanced C++/Graphics Tutorials on Youtube by MakingGameWithBen
// https://www.youtube.com/playlist?list=PLSPw4ASQYyymu3PfG9gxywSPghnSMiOAW
// Github: https://github.com/Barnold1953/GraphicsTutorials

// *Note - I made some of the private variables _var (he did that in the tutorial as well but 
// he corrected it later) but in C++ it's generally good practice to do name private variables
// as something different since the compiler uses _var as keywords (He and Cherno both use
// m_var). The ones following the m_var rule are variables that came later in the series

const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.3f;
const float PLAYER_SPEED = 8.0f;

MainGame::MainGame() :
	_gameState(GameState::PLAY),
	_fps(0.0f),
	_player(nullptr),
	_numHumansKilled(0),
	_numZombiesKilled(0)
{

}

MainGame::~MainGame() {
	for (size_t i = 0; i < _levels.size(); i++) {
		delete _levels[i];
	}
	for (size_t i = 0; i < _humans.size(); i++) {
		delete _humans[i];
	}
	for (size_t i = 0; i < _zombies.size(); i++) {
		delete _zombies[i];
	}
}

void MainGame::run() {
	initSystems();
	initLevel();
	myGameEngine::Music music = m_audioEngine.loadMusic("Sound/XYZ.ogg");
	music.play(-1);
	gameLoop();
}

void MainGame::initSystems() {
	myGameEngine::init();
	m_audioEngine.init();
	_window.create("ZombieGame", _screenWidth, _screenHeight, 0);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	initShaders(); 
	_agentSpriteBatch.init();
	_hudSpriteBatch.init();

	_spriteFont = new myGameEngine::SpriteFont("Fonts/chintzy.ttf", 64);

	_camera.init(_screenWidth, _screenHeight);
	_hudCamera.init(_screenWidth, _screenHeight);
	_hudCamera.setPosition(glm::vec2(_screenWidth / 2, _screenHeight /2 ));

	m_bloodParticleBatch = new myGameEngine::ParticleBatch2D;
	m_bloodParticleBatch->init(1000, 0.05f, 
		myGameEngine::ResourceManager::getTexture("Textures/particle.png"), 
		[](myGameEngine::Particle2D& particle, float deltaTime) {
			particle.position += particle.velocity * deltaTime;
			particle.color.a = (GLubyte)(particle.life * 255.0f);
		});
	m_particleEngine.addParticleBatch(m_bloodParticleBatch);
}

void MainGame::initLevel() {
	// Level 1
	_levels.push_back(new Level("Levels/level1.txt"));
	_currentLevel = 0;

	_player = new Player();
	_player->init(PLAYER_SPEED, _levels[_currentLevel]->getStartPlayerPos(), &_inputManager,
		&_camera, &_bullets);

	_humans.push_back(_player);

	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_int_distribution<int> randX(2, _levels[_currentLevel]->getWidth() - 1);
	std::uniform_int_distribution<int> randY(2, _levels[_currentLevel]->getHeight() - 1);

	// Add all the random humans
	for (int i = 0; i < _levels[_currentLevel]->getNumHumans(); i++) {
		_humans.push_back(new Human);
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
		_humans.back()->init(HUMAN_SPEED, pos);
	}

	// Add the Zombies
	const std::vector<glm::vec2>& zombiePositions = _levels[_currentLevel]->getStartZombiePos();
	for (size_t i = 0; i < zombiePositions.size(); i++) {
		_zombies.push_back(new Zombie);
		_zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
	}

	// Set up the player's guns
	const float BULLET_SPEED = 20.0f;
	_player->addGun(new Gun("Magnum", 10, 1, 5.0f, 30, BULLET_SPEED, m_audioEngine.loadSoundEffect("Sound/shots/pistol.wav")));
	_player->addGun(new Gun("Shotgun", 30, 12, 30.0f, 4, BULLET_SPEED, m_audioEngine.loadSoundEffect("Sound/shots/shotgun.wav")));
	_player->addGun(new Gun("MP5", 2, 1, 10.0f, 20, BULLET_SPEED, m_audioEngine.loadSoundEffect("Sound/shots/cg1.wav")));

}

void MainGame::initShaders() {
	_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
}

void MainGame::gameLoop() {

	myGameEngine::FpsLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(60000.0f);

	// We'll be doing semi-fixed timestepping
	// If we're taking longer than 6 steps to calculate everything to render then we should
	// slow down the framerate (to avoid spiral of death)
	const int MAX_PHYSICS_STEPS = 6;
	const float DESIRED_FPS = 60;
	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
	const float MAX_DELTA_TIME = 1.0f;

	float previousTicks = SDL_GetTicks();

	const float CAMERA_SCALE = 1.0f / 2.0f;
	_camera.setScale(CAMERA_SCALE);

	while (_gameState == GameState::PLAY) {

		fpsLimiter.begin();

		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - previousTicks;
		previousTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		checkVictory();
		_inputManager.update();
		processInput();

		// Physics loop
		int i = 0;
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			updateAgents(deltaTime);
			updateBullets(deltaTime);
			m_particleEngine.update(deltaTime);
			totalDeltaTime -= deltaTime;
			i++;
		}

		_camera.setPosition(_player->getPosition());
		_camera.update();
		_hudCamera.update();
		drawGame();

		_fps = fpsLimiter.end();
		std::cout << _fps << '\n';

	}
}

void MainGame::updateAgents(float deltaTime) {
	// Update all humans
	for (size_t i = 0; i < _humans.size(); i++) {
		_humans[i]->update(_levels[_currentLevel]->getLevelData(),
			_humans, _zombies, deltaTime);
	} 

	// Update all zombies
	for (size_t i = 0; i < _zombies.size(); i++) {
		_zombies[i]->update(_levels[_currentLevel]->getLevelData(),
			_humans, _zombies, deltaTime);
	}

	// Update human collisions
	for (size_t i = 0; i < _humans.size(); i++) {
		// Collide with other humans
		for (size_t j = i + 1; j < _humans.size(); j++) {
			_humans[i]->collideWithAgent(_humans[j]);
		}
	}

	// Update zombie collisions
	for (size_t i = 0; i < _zombies.size(); i++) {
		// Collide with other zombies
		for (size_t j = i + 1; j < _zombies.size(); j++) {
			_zombies[i]->collideWithAgent(_zombies[j]);
		}
		// Collide with humans
		for (size_t j = 1; j < _humans.size(); j++) {
			if (_zombies[i]->collideWithAgent(_humans[j])) {
				// Add new zombie 
				_zombies.push_back(new Zombie);
				_zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());
				// Delete the human
				delete _humans[j];
				_humans[j] = _humans.back();
				_humans.pop_back();

			}
		}
		// Collide with player
		if (_zombies[i]->collideWithAgent(_player)) {
			myGameEngine::fatalError("YOU LOSE");
		}
	}
}

void MainGame::updateBullets(float deltaTime) {
	// Update and collide with world
	for (size_t i = 0; i < _bullets.size();) {
		// If returns true, bullet collided with a wall
		if (_bullets[i].update(_levels[_currentLevel]->getLevelData(), deltaTime)) {
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}
		else {
			i++;
		}
	}

	bool wasBulletRemoved;

	// Collide with humans and zombies
	for (size_t i = 0; i < _bullets.size(); i++) {
		wasBulletRemoved = false;
		// Loops through zombies
		for (size_t j = 0; j < _zombies.size();) {
			// Check collision
			if (_bullets[i].collideWithAgent(_zombies[j])) {
				addBlood(_bullets[i].getPosition(), 5);

				// Damage zombie and kill if out of health
				if (_zombies[j]->applyDamage(_bullets[i].getDamage())) {
					delete _zombies[j];
					_zombies[j] = _zombies.back();
					_zombies.pop_back();
					_numZombiesKilled++;
				}
				else {
					j++;
				}

				// Remove the bullet
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
				wasBulletRemoved = true;
				i--; // Make sure we don't skip a bullet
				// Since bullet died, no need to loop through any more zombies
				break;
			}
			else {
				j++;
			}
		}
		if (wasBulletRemoved == false) {
			// Loop through humans, set j = 1 so it doesn't collide with player
			for (size_t j = 1; j < _humans.size();) {
				// Check collision
				if (_bullets[i].collideWithAgent(_humans[j])) {
					addBlood(_bullets[i].getPosition(), 5);

					// Damage human and kill if out of health
					if (_humans[j]->applyDamage(_bullets[i].getDamage())) {
						delete _humans[j];
						_humans[j] = _humans.back();
						_humans.pop_back();
					}
					else {
						j++;
					}

					// Remove the bullet
					_bullets[i] = _bullets.back();
					_bullets.pop_back();
					_numHumansKilled++;
					wasBulletRemoved = true;
					i--; // Make sure we don't skip a bullet
					// Since bullet died, no need to loop through any more zombies
					break;
				}
				else {
					j++;
				}
			}
		}
	}
}

void MainGame::checkVictory() {
	// We win if all zombies are dead
	if (_zombies.empty()) {
		std::printf("**** You win! ****\n You killed %d humans and %d zombies. There are %d/%d humans remaining", 
			_numHumansKilled, _numZombiesKilled, 
			_humans.size() - 1, _levels[_currentLevel]->getNumHumans());
		myGameEngine::fatalError("");
	}
}

void MainGame::processInput() {
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}
}

void MainGame::drawGame() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_textureProgram.use();
	glActiveTexture(GL_TEXTURE0);
	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	// Make sure the texture uses texture 0
	glUniform1i(textureUniform, 0);

	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	// Draw the level
	_levels[_currentLevel]->draw();

	_agentSpriteBatch.begin();

	const glm::vec2 agentDims(AGENT_RADIUS * 2.0f);

	// Draw the humans
	for (size_t i = 0; i < _humans.size(); i++) {
		if (_camera.isBoxInView(_humans[i]->getPosition(), agentDims)) {
			_humans[i]->draw(_agentSpriteBatch);
		}	
	}

	// Draw the zombies
	for (size_t i = 0; i < _zombies.size(); i++) {
		if (_camera.isBoxInView(_zombies[i]->getPosition(), agentDims)) {
			_zombies[i]->draw(_agentSpriteBatch);
		}
	}

	// Draw the bullets (we'll use agentSpriteBatch for bullets even though it isn't an agent)
	for (size_t i = 0; i < _bullets.size(); ++i) {
		_bullets[i].draw(_agentSpriteBatch); 
	}

	_agentSpriteBatch.end();
	_agentSpriteBatch.renderBatch();

	m_particleEngine.draw(&_agentSpriteBatch);

	drawHud();

	_textureProgram.unuse();

	_window.swapBuffer();
}

void MainGame::drawHud() {
	char buffer[256];

	glm::mat4 projectionMatrix = _hudCamera.getCameraMatrix();
	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	_hudSpriteBatch.begin();

	sprintf_s(buffer, "Num Humans %d", _humans.size());

	_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(0, 0),
		glm::vec2(0.5), 0.0f, myGameEngine::ColorRGBA8(255, 255, 255, 255));

	sprintf_s(buffer, "Num Zombies %d", _zombies.size());

	_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(0, 36),
		glm::vec2(0.5), 0.0f, myGameEngine::ColorRGBA8(255, 255, 255, 255));

	_hudSpriteBatch.end();
	_hudSpriteBatch.renderBatch();
}

void MainGame::addBlood(const glm::vec2& position, int numParticles) {

	static std::mt19937 randEngine(time(nullptr));
	static std::uniform_real_distribution<float> randAngle(0.0f, 2.0f * M_PI);

	glm::vec2 vel(2.0f, 0.0f);
	myGameEngine::ColorRGBA8 col(255, 0, 0, 255);

	for (int i = 0; i < numParticles; ++i) {
		m_bloodParticleBatch->addParticle(position, glm::rotate(vel, randAngle(randEngine)), col,
			30.0f);
	}
}