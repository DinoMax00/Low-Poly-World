#ifndef PARTICLE_RENDERER_HPP
#define PARTICLE_RENDERER_HPP

#include "../generator/ParticleEmitter.hpp"

class ParticleRenderer {
	std::vector<ParticleEmitter*> emitters;
public:
	ParticleRenderer() {
		ParticleEmitter::init();
	}

	void createEmitter(glm::vec3 color, glm::vec3 scale, glm::vec3 dropSpeed, 
					   glm::vec2 downleft, glm::vec2 upright, float emitHeight,
					   GLuint _nr_particles) {
		auto emitter = new ParticleEmitter(color, scale, dropSpeed, downleft, upright, emitHeight, _nr_particles);
		emitters.push_back(emitter);
	}

	void render(Camera* camera, glm::vec3 lightPos, glm::vec3 lightColor) {
		for (auto& emitter : emitters) {
			emitter->render(camera, lightPos, lightColor);
		}
	}

	~ParticleRenderer() {
		for (auto& i : emitters) delete i;
		std::vector<ParticleEmitter*>().swap(emitters);
		std::vector<glm::vec3>().swap(ParticleEmitter::trianglefront);
		std::vector<glm::vec3>().swap(ParticleEmitter::triangleback);
		delete ParticleEmitter::particleShader;
	}
};

#endif