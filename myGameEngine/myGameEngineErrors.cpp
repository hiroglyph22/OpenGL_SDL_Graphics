#include "myGameEngineErrors.h"
#include <cstdlib>
#include <iostream>
#include <SDL/SDL.h>

namespace myGameEngine {

	void fatalError(std::string errorString) {
		std::cout << errorString << std::endl;
		std::cout << "Enter anykey to quit...";
		int tmp;
		std::cin >> tmp;
		SDL_Quit();
		exit(69);
	}

}