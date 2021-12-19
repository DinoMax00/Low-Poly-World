#ifndef CLOUD_RENDERER
#define CLOUD_RENDERER

#include <vector>
#include <cmath>
#include "../objects/Cloud.hpp"
#include "../lib/shader.h"
#include "../lib/camera.h"
#include "../config.h"
#include <random>
#include <chrono>

class CloudRenderer {
private:
	Shader* cloudShader = nullptr;//  new Shader("cloudShader.vs", "cloudShader.fs");
	std::vector<Cloud>* clouds = nullptr;
	std::vector<glm::vec3> pos; // 云的坐标
	std::vector<float> rotate; // 旋转角度
	std::vector<glm::vec3> scale; // 缩放
	glm::vec3 moveSpeed; // 云的运动方向
	const int _MAP_SIZE = MAP_SIZE * 3;

public:

	friend class ShadowMapRender;

	CloudRenderer() {
		cloudShader = new Shader("shaders/cloudShader.vs", "shaders/cloudShader.fs");
		clouds = new std::vector<Cloud>();
	};

	CloudRenderer(std::vector<Cloud>* _clouds) {
		std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
		cloudShader = new Shader("shaders/cloudShader.vs", "shaders/cloudShader.fs");
		clouds = _clouds;
		pos.resize(clouds->size());
		rotate.resize(clouds->size());
		scale.resize(clouds->size());
		for (int i = 0; i < pos.size(); i++) { // 设置各个云坐标
			pos[i].y = CLOUD_BASE_HEIGHT + rng() % ((int)CLOUD_BASE_HEIGHT / 5);
			if (i % 4 == 0) {
				pos[i].x = 1.0 * _MAP_SIZE / 4 + rng() % (_MAP_SIZE / 4 - 5);
				pos[i].z = 1.0 * _MAP_SIZE / 4 + rng() % (_MAP_SIZE / 4 - 5);
			}
			else if (i % 4 == 1) {
				pos[i].x = 1.0 * _MAP_SIZE / 4 + rng() % (_MAP_SIZE / 4 - 5);
				pos[i].z = 3.0 * _MAP_SIZE / 4 + rng() % (_MAP_SIZE / 4 - 5);
			}
			else if (i % 4 == 2) {
				pos[i].x = 3.0 * _MAP_SIZE / 4 + rng() % (_MAP_SIZE / 4 - 5);
				pos[i].z = 1.0 * _MAP_SIZE / 4 + rng() % (_MAP_SIZE / 4 - 5);
			}
			else{
				pos[i].x = 3.0 * _MAP_SIZE / 4 + rng() % (_MAP_SIZE / 4 - 5);
				pos[i].z = 3.0 * _MAP_SIZE / 4 + rng() % (_MAP_SIZE / 4 - 5);
			}
		}
		for (auto& i : rotate)i = -60.0 + 120.0 * rng() / std::mt19937::max(); // 设置旋转角度
		for (auto& i : scale) {
			float tmp = CLOUD_SCALE + CLOUD_SCALE * 0.5 * rng() / std::mt19937::max(); // 设置 scale
			i.x = tmp * (1.0 + 0.5 * rng() / std::mt19937::max());
			i.y = tmp * (0.5 + 0.5 * rng() / std::mt19937::max());
			i.z = tmp * (0.8 + 0.15 * rng() / std::mt19937::max());
		}
		moveSpeed = glm::vec3(-MOVE_SPEED_BASE + MOVE_SPEED_BASE * 2 * rng() / std::mt19937::max(),
							0.0f, -MOVE_SPEED_BASE + MOVE_SPEED_BASE * 2 * rng() / std::mt19937::max()); // 设置云的运动方向
		// 防止速度过小
		if (moveSpeed.x < 0) moveSpeed.x = std::min(moveSpeed.x, -1.0f * MOVE_SPEED_BASE / 2);
		if (moveSpeed.x > 0) moveSpeed.x = std::max(moveSpeed.x, 1.0f * MOVE_SPEED_BASE / 2);
		if (moveSpeed.z < 0) moveSpeed.z = std::min(moveSpeed.z, -1.0f * MOVE_SPEED_BASE / 2);
		if (moveSpeed.z > 0) moveSpeed.z = std::max(moveSpeed.z, 1.0f * MOVE_SPEED_BASE / 2);
	}

	void render(Camera* camera, glm::vec3 lightDir, glm::vec3 lightColor) {
		glDisable(GL_CULL_FACE);   // 面剔除
		glEnable(GL_BLEND);
		cloudShader->use();

		glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)WINDOW_W / (float)WINDOW_H, NEAR_PLANE, FAR_PLANE);
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
			pos[i] += moveSpeed / scale[i] * glm::vec3(pos[i].y * pos[i].y / 400); // 根据云的大小 高度设置速度
			if (pos[i].x < 0) pos[i].x += _MAP_SIZE;
			else if (pos[i].x > _MAP_SIZE) pos[i].x -= _MAP_SIZE;

			if (pos[i].z < 0) pos[i].z += _MAP_SIZE;
			else if (pos[i].z > _MAP_SIZE) pos[i].z -= _MAP_SIZE;

			// std::cout << pos[i].x << " " << pos[i].y << " " << pos[i].z << std::endl;
			model = glm::translate(model, pos[i]);
			model = glm::rotate(model, glm::radians(rotate[i]), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, scale[i]);
			
				
			cloudShader->setMat4("model", model);
			for (int i = 0; i < Cloud::SPHERE_COUNT; i++) {
				glBindVertexArray(cloud.cloudVAOs[i]);
				glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
				// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDrawArrays(GL_TRIANGLES, 0, cloud.spherefaces[i].size());
			}
		}
		glEnable(GL_CULL_FACE);   // 面剔除
		glDisable(GL_BLEND);
	}
};

#endif
