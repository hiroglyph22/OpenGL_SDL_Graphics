#pragma once

#include <SDL/SDL_mixer.h>
#include <string>
#include <map>

namespace myGameEngine {

	class SoundEffect {
	public:
		// Allows only AudioEngine to access the private variables (we don't want people
		// to be able to access them without the Audio Engine)
		friend class AudioEngine;
		// @param loops: If loops == -1, loop forever, otherwise play it loops+1 times
		void play(int loops = 0);

	private:
		Mix_Chunk* m_chunk = nullptr;
	};

	class Music {
	public:
		friend class AudioEngine;
		// @param loops: If loops == -1, loop forever, otherwise play it loops times
		void play(int loops = 1);

		static void pause();
		static void stop();
		static void resume();
	private:
		Mix_Music* m_music = nullptr;
	};

	class AudioEngine
	{
	public:
		void init();
		void destroy();

		SoundEffect loadSoundEffect(const std::string& filePath);
		Music loadMusic(const std::string& filePath);
	private:

		// You can play multiple mix chunks at a time
		std::map<std::string, Mix_Chunk*> m_effectMap;
		// You can only play one music at a time
		std::map<std::string, Mix_Music*> m_musicMap;

		bool m_isInitialized = false;
	};

}

