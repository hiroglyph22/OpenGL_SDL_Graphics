#pragma once

#include <myGameEngine/Window.h>
#include <myGameEngine/GLSL_Program.h>
#include <myGameEngine/Camera2D.h>
#include <myGameEngine/InputManager.h>
#include <myGameEngine/SpriteBatch.h>
#include <myGameEngine/SpriteFont.h>
#include <myGameEngine/AudioEngine.h>
#include <myGameEngine/ParticleEngine2D.h>
#include <myGameEngine/ParticleBatch2D.h>

#include "Player.h"
#include "Level.h"
#include "Bullet.h"

class Zombie;

enum class GameState {PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();
private:
	void initSystems();
	void initLevel();
	void initShaders();
	void gameLoop();
	void updateAgents(float deltaTime);
	void updateBullets(float deltaTime);
	void checkVictory();
	void processInput();
	void drawGame();
	void drawHud();
	void addBlood(const glm::vec2& position, int numParticles);

	myGameEngine::Window _window;
	myGameEngine::GLSL_Program _textureProgram;
	myGameEngine::InputManager _inputManager;
	myGameEngine::Camera2D _camera;
	myGameEngine::Camera2D _hudCamera;
	myGameEngine::SpriteBatch _agentSpriteBatch;
	myGameEngine::SpriteBatch _hudSpriteBatch;
	myGameEngine::ParticleEngine2D m_particleEngine;
	myGameEngine::ParticleBatch2D* m_bloodParticleBatch;

	std::vector<Level*> _levels;

	int _screenWidth = 1024, _screenHeight = 768;
	float _fps;
	int _currentLevel;

	Player* _player;
	std::vector<Human*> _humans;
	std::vector<Zombie*> _zombies;
	std::vector<Bullet> _bullets;

	int _numHumansKilled; // Humans killed by player
	int _numZombiesKilled; // Zombies killed by player

	myGameEngine::SpriteFont* _spriteFont;
	
	myGameEngine::AudioEngine m_audioEngine;

	GameState _gameState;
};