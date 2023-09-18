#pragma once

#include "myGameEngine.h"
#include "Window.h"
#include "InputManager.h"
#include "ScreenList.h"
#include <memory>

namespace myGameEngine {

	class ScreenList;
	class IGameScreen;

	class IMainGame
	{
	public:
		IMainGame();

		void run();
		void exitGame();
		virtual void onInit() = 0;
		virtual void addScreens() = 0;
		virtual void onExit() = 0;

		const float getFps() const {
			return m_fps;
		}

		void onSDLEvent(SDL_Event& evnt);

	protected:
		bool init();
		bool initSystems();

		virtual void update();
		virtual void draw();

		std::unique_ptr<ScreenList> m_screenList = nullptr;
		IGameScreen* m_currentScreen = nullptr;
		bool m_isRunning = false;
		float m_fps = 0.0f;;
		Window m_window;
		InputManager m_inputManager;
	};

}

