#include <iostream>
#include "MainGame.h"

// main function needs argc and argv because SDL already has its own main function
int main(int argc, char** argv) {
	MainGame mainGame;
	mainGame.run();

	return 0;
}