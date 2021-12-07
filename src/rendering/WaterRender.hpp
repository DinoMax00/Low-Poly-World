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
	unsigned int VAO = 0, VBO = 0, FBO = 0;

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

	void render(Camera* camera, Light* light) {
		time += wave_speed;

		/// prepare shader
		water_shader->use();
		water_shader->setFloat("waveTime", time);
		water_shader->setFloat("height", WATER_HEIGHT);

		water_shader->setVec3("lightColour", light->get_color());
		water_shader->setVec3("lightDirection", light->get_direction());
		water_shader->setVec2("lightBias", light->get_bias());
		
		glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)WINDOW_H / (float)WINDOW_W, 0.1f, 1000.0f);
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		water_shader->setMat4("projectionViewMatrix", projection * view * model);
		water_shader->setVec3("cameraPos", camera->Position);
		// water_shader->setVec2("projectionViewMatrix", glm::vec2(NEAR_PLANE, FALR_PLANE));

		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
		glBindVertexArray(this->VAO);
		glDrawArrays(GL_TRIANGLES, 0, buffer_len / 3);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// glDrawElements(GL_TRIANGLES, idc_len, GL_UNSIGNED_INT, 0);
	}

private:

	void preWork() {
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, buffer_len * sizeof(glm::vec2),
			buffer, GL_DYNAMIC_DRAW);

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





