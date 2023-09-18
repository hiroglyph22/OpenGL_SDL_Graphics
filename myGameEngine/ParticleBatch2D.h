#pragma once

#include <functional>
#include <glm/glm.hpp>
#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"

namespace myGameEngine {

	class Particle2D {
	public:
		glm::vec2 position = glm::vec2(0.0f);
		glm::vec2 velocity = glm::vec2(0.0f);
		ColorRGBA8 color;
		float life = 0.0f;
		float width = 0.0f;
	};

	inline void defaultParticleUpdate(Particle2D& particle, float deltaTime) {
		particle.position += particle.velocity * deltaTime;
	}

	class ParticleBatch2D
	{
	public:
		
		~ParticleBatch2D();

		void init(int maxParticles, float decayRate, GLTexture texture, 
			std::function<void(Particle2D&, float)> updateFunc = defaultParticleUpdate);
		void update(float deltaTime);
		void draw(SpriteBatch* spriteBatch);
		void addParticle(const glm::vec2& position,
						 const glm::vec2& velocity,
						 const ColorRGBA8& color,
						 float width);
	private:
		// We aren't sorting the particles and we're going to be doing linear sort,
		// which is slow, so we're going to keep track of the last free particle so we only
		// need to increment the index by one to find space for the new particle
		int findFreeParticle();	 
		std::function<void(Particle2D&, float)> m_updateFunc;

		float m_decayRate = 0.1f;
		// We're going to be using a buffer for particles and when a particle dies we'll 
		// just disable it so that we don't have to use "new" keyword multiple times
		Particle2D* m_particles = nullptr;
		int m_maxParticles = 0;
		int m_lastFreeParticle = 0;
		GLTexture m_texture;
	};

}

