#pragma once

#include <myGameEngine/IMainGame.h>
#include "GameplayScreen.h"

class App : public myGameEngine::IMainGame
{
public:
	~App();

	virtual void onInit() override;
	virtual void addScreens() override;
	virtual void onExit() override;
private:
	std::unique_ptr<GameplayScreen> m_gameplayScreen = nullptr;
};

