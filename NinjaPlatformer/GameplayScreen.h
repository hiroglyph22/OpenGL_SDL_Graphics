#pragma once

#include <myGameEngine/IGameScreen.h>

class GameplayScreen : public myGameEngine::IGameScreen
{
public:
	virtual int getNextScreenIndex() const override;
	virtual int getPreviousScreenIndex() const override;

	virtual void build() override;
	virtual void destroy() override;
	// Called when a screen enters and exits focus
	virtual void onEntry() override;
	virtual void onExit() override;
	// Called in the main game loop
	virtual void update() override;
	virtual void draw() override;

private:
	void checkInput();
};

