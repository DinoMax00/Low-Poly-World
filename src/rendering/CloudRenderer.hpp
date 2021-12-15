#ifndef CLOUD_RENDERER
#define CLOUD_RENDERER

#include <vector>
#include <cmath>
#include "../objects/Cloud.hpp"
#include "../lib/shader.h"
#include "../lib/camera.h"
#include "../config.h"
#include <ctime>

class CloudRenderer {
private:
	Shader* cloudShader = nullptr;//  new Shader("cloudShader.vs", "cloudShader.fs");
	std::vector<Cloud>* clouds = nullptr;
	std::vector<glm::vec3> pos; // 云的坐标
	std::vector<float> rotate; // 旋转角度
	std::vector<float> scale; // 缩放
	glm::vec3 moveSpeed; // 云的运动方向

	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;
	const float MOVE_SPEED_BASE = 0.002f;

public:
	CloudRenderer() {
		cloudShader = new Shader("shaders/cloudShader.vs", "shaders/cloudShader.fs");
		clouds = new std::vector<Cloud>();
	};

	CloudRenderer(std::vector<Cloud>* _clouds) {
		cloudShader = new Shader("shaders/cloudShader.vs", "shaders/cloudShader.fs");
		clouds = _clouds;
		pos.resize(clouds->size());
		rotate.resize(clouds->size());
		scale.resize(clouds->size());
		for (auto& i : pos) { // 设置各个云坐标
			i.x = MAP_SIZE / 2 + (rand() * 2 - RAND_MAX) % ((int)MAP_SIZE / 2 - 5);
			i.y = AMPLITUDE + 2 + rand() % 5;
			i.z = MAP_SIZE / 2 + (rand() * 2 - RAND_MAX) % ((int)MAP_SIZE / 2 - 5);
		}
		for (auto& i : rotate)i = rand(); // 设置旋转角度
		for (auto& i : scale) i = 1.0 + 0.5 * rand() / RAND_MAX; // 设置 scale
		moveSpeed = glm::vec3(-MOVE_SPEED_BASE + MOVE_SPEED_BASE * 2 * rand() / RAND_MAX, 
							0.0f, -MOVE_SPEED_BASE + MOVE_SPEED_BASE * 2 * rand() / RAND_MAX); // 设置云的运动方向
		// 防止速度过小
		if (moveSpeed.x < 0) moveSpeed.x = std::min(moveSpeed.x, -1.0f * MOVE_SPEED_BASE / 2);
		if (moveSpeed.x > 0) moveSpeed.x = std::max(moveSpeed.x, 1.0f * MOVE_SPEED_BASE / 2);
		if (moveSpeed.z < 0) moveSpeed.z = std::min(moveSpeed.z, -1.0f * MOVE_SPEED_BASE / 2);
		if (moveSpeed.z > 0) moveSpeed.z = std::max(moveSpeed.z, 1.0f * MOVE_SPEED_BASE / 2);
	}

	void render(Camera* camera, glm::vec3 lightDir, glm::vec3 lightColor) {
		glDisable(GL_MULTISAMPLE); // 抗锯齿
		glDisable(GL_CULL_FACE);   // 面剔除
		cloudShader->use();

		glm::mat4 projection =
			glm::perspective(glm::radians(camera->Zoom),
				(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		cloudShader->setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera->GetViewMatrix();
		cloudShader->setMat4("view", view);

		// cloudShader.setVec3("light.position", lightPos); 
		cloudShader->setVec3("viewPos", camera->Position);
		cloudShader->setVec3("light.direction", lightDir.x, lightDir.y, lightDir.z);

		glm::vec3 diffuseColor = lightColor * glm::vec3(0.6f); // decrease the influen 
		glm::vec3 ambientColor = lightColor *glm::vec3(0.6f); // low influence
		cloudShader->setVec3("light.ambient", ambientColor);
		cloudShader->setVec3("light.diffuse", diffuseColor);

		cloudShader->setVec3("material.ambient", glm::vec3(1.0f));
		cloudShader->setVec3("material.diffuse", glm::vec3(0.6f));

		for (int i = 0; i < clouds->size(); i++) {
			Cloud cloud = (*clouds)[i];
			glm::mat4 model = glm::mat4(1.0f);
			pos[i] += moveSpeed / scale[i] * glm::vec3(pos[i].y / 20); // 根据云的大小 高度设置速度
			if (pos[i].x < 0) pos[i].x += MAP_SIZE;
			else if (pos[i].x > MAP_SIZE) pos[i].x -= MAP_SIZE;

			if (pos[i].z < 0) pos[i].z += MAP_SIZE;
			else if (pos[i].z > MAP_SIZE) pos[i].z -= MAP_SIZE;

			model = glm::translate(model, pos[i]);
			model = glm::scale(model, glm::vec3(1.0f * scale[i], 1.0f * scale[i], 0.9f * scale[i]));// make sure to initialize matrix to identity matrix first
			model = glm::rotate(model, glm::radians(rotate[i]), glm::vec3(0.0f, 1.0f, 0.0f));
				
			cloudShader->setMat4("model", model);
			for (int i = 0; i < Cloud::SPHERE_COUNT; i++) {
				glBindVertexArray(cloud.cloudVAOs[i]);
				glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
				// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDrawArrays(GL_TRIANGLES, 0, cloud.spherefaces[i].size());
			}
		}
		glEnable(GL_MULTISAMPLE); // 抗锯齿
		glEnable(GL_CULL_FACE);   // 面剔除
	}
};

#endif
