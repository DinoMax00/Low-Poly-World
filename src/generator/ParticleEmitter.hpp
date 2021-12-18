#ifndef PARTICLE_EMITER_HPP
#define PARTICLE_EMITER_HPP
// https://www.raywenderlich.com/2704-opengl-es-particle-system-tutorial-part-1-3
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../lib/shader.h"
#include "../lib/camera.h"
#include <vector>
#include <iostream>

struct Particle {
	void reset(glm::vec3 _dropSpeed, glm::vec2 downLeft, glm::vec2 upRight, float emitHeight) {
		active = true;
		position = glm::vec3(downLeft.x + (upRight.x - downLeft.x) * rand() / RAND_MAX,
							emitHeight - emitHeight / 2 * rand() / RAND_MAX,
							downLeft.y + (upRight.y - downLeft.y) * rand() / RAND_MAX);
		dropSpeed = _dropSpeed;
		float tmp = 0.2 + 0.7 * rand() / RAND_MAX;
		rotate = glm::vec3(sqrt(1.0f - tmp * tmp), 0.0f, tmp);
		rotateRadiansY = 1.0 * rand();
		rotateSpeedZ = 40.0 + 60.0 * rand() / RAND_MAX;
		scale = glm::vec3(0.3 + 0.7 * rand() / RAND_MAX, 0.3 + 0.7 * rand() / RAND_MAX, 0.3 + 0.7 * rand() / RAND_MAX);
	}
	bool active;
	glm::vec3 position; // center position
	glm::vec3 rotate;
	glm::vec3 scale;
	float rotateRadiansY;
	float rotateSpeedZ;
	glm::vec3 dropSpeed;
};

class ParticleEmitter {
public:
	static std::vector<glm::vec3> trianglefront, triangleback;
	static Shader* particleShader;
	static GLuint particleVAOfront, particleVBOfront;
	static GLuint particleVAOback, particleVBOback;
	
private:
	GLuint nr_particles; // 最多粒子个数
	std::vector<Particle> particles; // 粒子容器
	GLuint lastUsedParticle; // 用于粒子重新生成
	glm::vec3 color; // 粒子颜色
	glm::vec3 scale; // 总体的粒子大小
	glm::vec3 dropSpeed; // 总体下落速度
	float emitHeight; //  发射的位置

	// x-z 面的边界
	glm::vec2 rangeDownLeft;
	glm::vec2 rangeUpRight;

public:
	ParticleEmitter() {
		nr_particles = 20;
		lastUsedParticle = 0;
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		scale = glm::vec3(0.5f);
		dropSpeed = glm::vec3(0.0, -0.0003f, 0.0);
		rangeDownLeft = glm::vec2(0.0f, 0.0f);
		rangeUpRight = glm::vec2(1.0f, 1.0f);
		emitHeight = 1.0f;
		particles.resize(nr_particles);
		for (auto& i : particles) i.reset(dropSpeed, rangeDownLeft, rangeUpRight, emitHeight);
	}

	ParticleEmitter(glm::vec3 _color, glm::vec3 _scale, glm::vec3 _dropSpeed, glm::vec2 _downleft, glm::vec2 _upright,
					float _emitHeight, GLuint _nr_particles) {
		nr_particles = _nr_particles;
		lastUsedParticle = 0;
		color = _color;
		scale = _scale;
		dropSpeed = _dropSpeed;
		rangeDownLeft = _downleft;
		rangeUpRight = _upright;
		emitHeight = _emitHeight;
		particles.resize(nr_particles);
		for (auto& i : particles) i.reset(dropSpeed, rangeDownLeft, rangeUpRight, emitHeight);
	}

	static void init() {
		particleShader = new Shader("shaders/particle.vs", "shaders/particle.fs");
		//init 3 vertices
		glm::vec3 v1 = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 v2 = glm::vec3(0.0f, 0.0f, 0.6f);
		glm::vec3 v3 = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 norm = glm::cross(v2 - v1, v3 - v1);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		trianglefront.push_back(v1), trianglefront.push_back(norm);
		trianglefront.push_back(v2), trianglefront.push_back(norm);
		trianglefront.push_back(v3), trianglefront.push_back(norm);
		trianglefront.shrink_to_fit();
		
		// color = glm::vec3(1.0f, 0.0f, 0.0f);
		triangleback.push_back(v3), triangleback.push_back(-norm);
		triangleback.push_back(v2), triangleback.push_back(-norm);
		triangleback.push_back(v1), triangleback.push_back(-norm);
		triangleback.shrink_to_fit();

		glGenVertexArrays(1, &particleVAOfront);
		glGenBuffers(1, &particleVBOfront);

		glBindBuffer(GL_ARRAY_BUFFER, particleVBOfront);
		glBufferData(GL_ARRAY_BUFFER, trianglefront.size() * sizeof(glm::vec3),
			&trianglefront[0], GL_DYNAMIC_DRAW);
		
		glBindVertexArray(particleVAOfront);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void*)sizeof(glm::vec3));
		glEnableVertexAttribArray(1);
		//

		glGenVertexArrays(1, &particleVAOback);
		glGenBuffers(1, &particleVBOback);

		glBindBuffer(GL_ARRAY_BUFFER, particleVBOback);
		glBufferData(GL_ARRAY_BUFFER, triangleback.size() * sizeof(glm::vec3),
			&triangleback[0], GL_DYNAMIC_DRAW);

		glBindVertexArray(particleVAOback);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void*)sizeof(glm::vec3));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void*)(2 * sizeof(glm::vec3)));
		glEnableVertexAttribArray(2);

		std::vector<glm::vec3>().swap(trianglefront);
		std::vector<glm::vec3>().swap(triangleback);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	GLuint firstUnusedParticle() {
		// Search from last used particle, this will usually return almost instantly
		for (GLuint i = lastUsedParticle; i < nr_particles; ++i) {
			if (particles[i].active == false) {
				lastUsedParticle = i;
				return i;
			}
		}
		// otherwise, do a linear search
		for (GLuint i = 0; i < lastUsedParticle; i++) {
			if (particles[i].active == false) {
				lastUsedParticle = i;
				return i;
			}
		}
		return -1;
	}

	void respawnParticle(Particle& particle) {
		particle.reset(dropSpeed, rangeDownLeft, rangeUpRight, emitHeight);
	}

	void updateParticles() {
		GLuint nr_new_particles = 3;
		// Add new particles
		for (GLuint i = 0; i < nr_new_particles; ++i) {
			int unusedParticle = firstUnusedParticle();
			if(unusedParticle != -1) respawnParticle(particles[unusedParticle]);
		}
		// update all particles
		for (GLuint i = 0; i < nr_particles; ++i) {
			Particle& p = particles[i];
			if (rangeDownLeft.x <= p.position.x && p.position.x <= rangeUpRight.x 
				&& rangeDownLeft.y <= p.position.z && p.position.z <= rangeUpRight.y
				&& p.position.y > 0.0f) {
				// particle is alive, thus update
				p.position += p.dropSpeed;
			} else p.active = false;
		}
	}

	void render(Camera* camera, glm::vec3 lightPos, glm::vec3 lightColor) {
		glEnable(GL_CULL_FACE);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		particleShader->use();

		particleShader->setVec3("color", color);
			
		glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)WINDOW_W / (float)WINDOW_H, NEAR_PLANE, FAR_PLANE);
		particleShader->setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera->GetViewMatrix();
		particleShader->setMat4("view", view);

		// cloudShader.setVec3("light.position", lightPos); 
		particleShader->setVec3("viewPos", camera->Position);
		particleShader->setVec3("light.direction", lightPos.x, lightPos.y, lightPos.z);

		// glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influen 
		// glm::vec3 ambientColor = lightColor * glm::vec3(0.8f); // low influence
		particleShader->setVec3("light.ambient", lightColor * glm::vec3(0.9f));
		particleShader->setVec3("light.diffuse", lightColor * glm::vec3(0.8f));

		particleShader->setVec3("material.ambient", 1.0f, 1.0f, 1.0f);
		particleShader->setVec3("material.diffuse", 1.0f, 1.0f, 1.0f);
		
		// std::cout << "ok" << std::endl;
		updateParticles();
		for (Particle particle : particles) {
			if (particle.active) {
				//std::cout << particle.position.x << " " << particle.position.y << " " << particle.position.z << std::endl;
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, particle.position);
				model = glm::rotate(model, glm::radians((float)glfwGetTime() * particle.rotateSpeedZ), particle.rotate);
				model = glm::rotate(model, glm::radians(particle.rotateRadiansY), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::scale(model, particle.scale);
				model = glm::scale(model, scale);
				
				particleShader->setMat4("model", model);
				
				glCullFace(GL_BACK); // 剔除背面
				glBindVertexArray(particleVAOfront);
				glDrawArrays(GL_TRIANGLES, 0, 3);

				glBindVertexArray(particleVAOback);
				glDrawArrays(GL_TRIANGLES, 0, 3);

				glBindVertexArray(0);
			}
		}
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
	}
};

// static 变量初始化
std::vector<glm::vec3>ParticleEmitter::trianglefront = std::vector<glm::vec3>();
std::vector<glm::vec3>ParticleEmitter::triangleback = std::vector<glm::vec3>();
Shader*  ParticleEmitter::particleShader = nullptr;
GLuint ParticleEmitter::particleVAOfront = 0;
GLuint ParticleEmitter::particleVAOback = 0;
GLuint ParticleEmitter::particleVBOfront = 0;
GLuint ParticleEmitter::particleVBOback = 0;

#endif