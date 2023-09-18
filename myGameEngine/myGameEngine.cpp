#include "myGameEngine.h"

#include <SDL/SDL.h>
#include <GL/glew.h>

// Following the Advanced C++/Graphics Tutorials on Youtube by MakingGameWithBen
// https://www.youtube.com/playlist?list=PLSPw4ASQYyymu3PfG9gxywSPghnSMiOAW
// Github: https://github.com/Barnold1953/GraphicsTutorials

// *Note - I made some of the private variables _var (he did that in the tutorial as well but 
// he corrected it later) but in C++ it's generally good practice to do name private variables
// as something different since the compiler uses _var as keywords (He and Cherno both use
// m_var). The ones following the m_var rule are variables that came later in the series

namespace myGameEngine {

	int init() {
		SDL_Init(SDL_INIT_EVERYTHING);

		// Creates 2 window panes, one for clearing and one for drawing, 
		// which will stop flickering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}

}