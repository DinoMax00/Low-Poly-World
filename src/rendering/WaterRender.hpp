#pragma once

#include "../lib/shader.h"
#include "../lib/camera.h"
#include "../objects/Light.hpp"
#include "../config.h"

/**
 * 水的渲染.
 */

class WaterRender {
private:
	unsigned int VAO = 0, VBO = 0;

	float wave_speed;
	float time;
	Shader* water_shader;
	unsigned int buffer_len;
	glm::vec2* buffer;
public:

	WaterRender(unsigned int _buffer_len, glm::vec2* _buffer) {
		buffer_len = _buffer_len;
		buffer = _buffer;
		wave_speed = WAVE_SPEED;
		time = 0;
		water_shader = new Shader("shaders\\water.vs", "shaders\\water.fs");

		preWork();
	}

	~WaterRender() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	void render(Camera* camera, Light* light, unsigned int reflection_color, unsigned int refraction_color, unsigned int refraction_depth) {
		time += wave_speed;

		glBindVertexArray(this->VAO);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		/// prepare shader
		water_shader->use();
		water_shader->setFloat("waveTime", time);
		water_shader->setFloat("height", WATER_HEIGHT);

		water_shader->setVec3("lightColour", light->get_color());
		water_shader->setVec3("lightDirection", light->get_direction());
		water_shader->setVec2("lightBias", light->get_bias());
		
		glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)WINDOW_W / (float)WINDOW_H, NEAR_PLANE, FAR_PLANE);
		glm::mat4 view = camera->GetViewMatrix();

		water_shader->setMat4("projectionViewMatrix", projection * view);
		water_shader->setVec3("cameraPos", camera->Position);
		water_shader->setVec2("nearFarPlanes", glm::vec2(NEAR_PLANE, FAR_PLANE));
		
		// 绑定texture
		water_shader->setInt("reflectionTexture", 0);
		water_shader->setInt("refractionTexture", 1);
		water_shader->setInt("depthTexture", 2);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, reflection_color);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, refraction_color);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, refraction_depth);
		
		glDrawArrays(GL_TRIANGLES, 0, buffer_len / 3);
		
		glBindVertexArray(0);
		glDisable(GL_BLEND);
		glUseProgram(0);
	}

private:

	void preWork() {
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, buffer_len * sizeof(glm::vec2), buffer, GL_DYNAMIC_DRAW);
		// for (int i = 0; i < buffer_len; i += 3) std::cout << buffer[i].x << " " << buffer[i].y << "::" << buffer[i + 1].x << " " << buffer[i + 1].y << buffer[i + 2].x << " " << buffer[i + 2].y << std::endl;

		// position attribute
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec2), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec2), (void*)(1 * sizeof(glm::vec2)));
		glEnableVertexAttribArray(1);

		// 解除绑定
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};





