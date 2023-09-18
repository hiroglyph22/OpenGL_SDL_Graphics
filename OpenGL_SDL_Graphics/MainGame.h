#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <vector>

#include <myGameEngine/GLSL_Program.h>
#include <myGameEngine/GLTexture.h>
#include <myGameEngine/Sprite.h>
#include <myGameEngine/Window.h>
#include <myGameEngine/myGameEngine.h>
#include <myGameEngine/Camera2D.h>
#include <myGameEngine/SpriteBatch.h>
#include <myGameEngine/InputManager.h>
#include <myGameEngine/Timing.h>

#include "Bullet.h"

enum class GameState {PLAY, EXIT};

class MainGame
{
public:
	MainGame();

	void run();

private:
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();

	myGameEngine::Window _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;

	myGameEngine::GLSL_Program _colorProgram;
	myGameEngine::Camera2D _camera;

	myGameEngine::SpriteBatch _spriteBatch;

	myGameEngine::InputManager _inputManager;
	myGameEngine::FpsLimiter _fpsLimiter;

	std::vector<Bullet> _bullets;

	float _maxFPS;
	float _fps;
	float _time;
};

