#include "IMainGame.h"
#include "Timing.h"
#include "IGameScreen.h"

namespace myGameEngine {

	IMainGame::IMainGame() {
		m_screenList = std::make_unique<ScreenList>(this);
	}

	void IMainGame::run() {
		if (!init()) return;

		FpsLimiter limiter;
		limiter.setMaxFPS(60.0f);

		m_isRunning = true;
		while (m_isRunning) {
			limiter.begin();

			m_inputManager.update();
			update();
			draw();

			m_fps = limiter.end();
			m_window.swapBuffer();	
		}
	}

	void IMainGame::exitGame() {
		m_currentScreen->onExit();
		if (m_screenList) {
			m_screenList->destroy();
			//m_screenList.reset();
		}
		m_isRunning = false;
	}

	void IMainGame::onSDLEvent(SDL_Event& evnt) {
		switch (evnt.type) {
		case SDL_QUIT:
			m_isRunning = false;
			break;
		case SDL_MOUSEMOTION:
			m_inputManager.setMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			m_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}

	bool IMainGame::init() {
		myGameEngine::init();
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1); // Might already be set by default
		if(!initSystems()) return false;
		onInit();
		addScreens();

		m_currentScreen = m_screenList->getCurrent();
		m_currentScreen->onEntry();
		m_currentScreen->setRunning();

		return true;
	}

	bool IMainGame::initSystems() {
		m_window.create("Default", 1920, 1080, 0);
		return true;
	}

	void IMainGame::update() {
		if (m_currentScreen) {
			switch (m_currentScreen->getState()) {
			case ScreenState::RUNNING:
				m_currentScreen->update();
				break;
			case ScreenState::CHANGE_NEXT:
				m_currentScreen->onExit();
				m_currentScreen = m_screenList->moveNext();
				if (m_currentScreen) {
					m_currentScreen->setRunning();
					m_currentScreen->onEntry();
				}
				break;
			case ScreenState::CHANGE_PREVIOUS:
				m_currentScreen->onExit();
				m_currentScreen = m_screenList->movePrevious();
				if (m_currentScreen) {
					m_currentScreen->setRunning();
					m_currentScreen->onEntry();
				}
				break;
			case ScreenState::EXIT_APPLICATION:
				exitGame();
				break;
			default:
				break;
			}
		}
		else {
			exitGame();
		}
	}

	void IMainGame::draw() {
		// Prob don't need glViewport but just in case
		glViewport(0, 0, m_window.getScreenWidth(), m_window.getScreenHeight());
		if (m_currentScreen && m_currentScreen->getState() == ScreenState::RUNNING) {
			m_currentScreen->draw();
		}
	}
}