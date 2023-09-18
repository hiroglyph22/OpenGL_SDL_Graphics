#include "App.h"
#include <myGameEngine/ScreenList.h>

App::~App() {
	m_screenList->destroy();
	m_currentScreen = nullptr;
}

void App::onInit() {

}

void App::addScreens() {
	m_gameplayScreen = std::make_unique<GameplayScreen>();
	m_screenList->addScreen(m_gameplayScreen.get());
	m_screenList->setScreen(m_gameplayScreen->getScreenIndex());
}

void App::onExit() {

}