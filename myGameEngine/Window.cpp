#include "Window.h"
#include "myGameEngineErrors.h"

namespace myGameEngine {

	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags) {

		Uint32 flags = SDL_WINDOW_OPENGL;

		if (currentFlags & INVISIBLE) {
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN) {
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlags & BORDERLESS) {
			flags |= SDL_WINDOW_BORDERLESS;
		}

		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);

		if (_sdlWindow == nullptr) {
			fatalError("SDL Window could not be created!");
		}

		SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
		if (glContext == nullptr) {
			fatalError("SDL_GL context could not be created!");
		}

		GLenum error = glewInit();
		if (error != GLEW_OK) {
			fatalError("Could not initialize glew!");
		}

		glClearColor(0, 0, 1, 1);

		// Check the OpenGL version
		std::printf("*** Open GL Version: %s *** \n", glGetString(GL_VERSION));

		// Nvidia already turned Vsync on but you set it manually like this
		SDL_GL_SetSwapInterval(0);

		// Enable alpha blending - we need it for pngs to set alpha to 0 where the
		// png sets it to 0 so we don't have a black background but have a 
		// transparent on instead
		glEnable(GL_BLEND);
		// First argument is what you want to multiple the color by (in our case
		// alpha, and then how much of the background do you want to see (1-alpha in
		// our case so that when the color of the sprite has alpha of 1 we're not
		// drawing the background behind it because we're not going to see it anyways
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	void Window::swapBuffer() {
		SDL_GL_SwapWindow(_sdlWindow);
	}

}