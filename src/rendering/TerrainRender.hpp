#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../objects/Light.hpp"
#include "../lib/camera.h"
#include "../lib/shader.h"
#include "../config.h"

/**
 * 地形渲染.
 */

class TerrainRender {
private:
	unsigned int VAO, VBO, EBO;
	Shader* terrain_shader;

	int buffer_len, idc_len;
	glm::vec3* buffer;
	int* indices;

public:

	TerrainRender(unsigned int _buffer_len, glm::vec3* _buffer, unsigned int _idc_len, int* _indices)
	: buffer_len(_buffer_len), buffer(_buffer), idc_len(_idc_len), indices(_indices)
	{
		terrain_shader =  new Shader("shaders\\terrain.vs", "shaders\\terrain.fs");

		preWork();
	}

	/**
	 * 渲染地形.
	 * 之后考虑把light封装成一个类
	 */
	void render(Camera* camera, Light* light) {
		terrain_shader->use();
		terrain_shader->setVec3("lightColor", light->get_color());
		terrain_shader->setVec3("lightDirection", light->get_direction());
		terrain_shader->setVec2("lightBias", light->get_bias());
		terrain_shader->setVec3("viewPos", camera->Position);

		glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)WINDOW_H / (float)WINDOW_W, 0.1f, 1000.0f);
		terrain_shader->setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera->GetViewMatrix();
		terrain_shader->setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		terrain_shader->setMat4("model", model);

		terrain_shader->setFloat("opc", 1.0f);

		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, idc_len, GL_UNSIGNED_INT, 0);
	}

private:
	void preWork() {
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, buffer_len * sizeof(glm::vec3),
			buffer, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, idc_len * sizeof(int),
			indices, GL_DYNAMIC_DRAW);
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void*)(1 * sizeof(glm::vec3)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void*)(2 * sizeof(glm::vec3)));
		glEnableVertexAttribArray(2);

		// 解除绑定
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};
