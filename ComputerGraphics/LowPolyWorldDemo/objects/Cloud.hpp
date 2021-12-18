#ifndef CLOUD
#define CLOUD

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>

class Cloud {
	const float PI = 3.14159265358979323846f;
	const int SEGMENT_COUNT = 15;
	int Y_SEGMENTS, X_SEGMENTS;

public:
	static const int SPHERE_COUNT = 3;
	std::vector<glm::vec3> sphereVertices[3];
	std::vector<glm::vec3> spherefaces[3]; // pos-vec3 color-vec3 faces-vec3
	glm::vec3 internalPositions[SPHERE_COUNT]; // relative position in model
	float radius[SPHERE_COUNT];

public:
	unsigned int cloudVAOs[SPHERE_COUNT], cloudVBOs[SPHERE_COUNT];
public:
	void genVertices(int idx) {
		Y_SEGMENTS = (rand() % SEGMENT_COUNT / 2 + SEGMENT_COUNT) * sqrt(sqrt(radius[idx]));
		X_SEGMENTS = (rand() % SEGMENT_COUNT / 2 + SEGMENT_COUNT) * sqrt(sqrt(radius[idx]));
		for (int y = 0; y <= Y_SEGMENTS; y++) {
			for (int x = 0; x <= X_SEGMENTS; x++) {
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = internalPositions[idx].x + radius[idx] * std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = internalPositions[idx].y + radius[idx] * std::cos(ySegment * PI);
				float zPos = internalPositions[idx].z + radius[idx] * std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				if (yPos < 1e-5) yPos *= std::fabs(std::cos(yPos)) * 0.3;
				else {
					/*if (yPos > 0.3) yPos /= std::fabs(std::cos(yPos)) * 2;
					else yPos /= std::fabs(std::cos(yPos)) * 3;*/
				}

				if (std::fabs(yPos) > 1e-5 && std::fabs(zPos) > 1e-5 && std::fabs(xPos) > 1e-5) {
					xPos += (yPos > 1e-5 ? 0.6 * std::pow(radius[idx], 2) : 0.1) / SEGMENT_COUNT * (std::rand() * 2 - RAND_MAX) / RAND_MAX;
					yPos += (yPos > 1e-5 ? 0.6 * std::pow(radius[idx], 2) : 0.1) / SEGMENT_COUNT * (std::rand() * 2 - RAND_MAX) / RAND_MAX;
					zPos += (yPos > 1e-5 ? 0.6 * std::pow(radius[idx], 2) : 0.1) / SEGMENT_COUNT * (std::rand() * 2 - RAND_MAX) / RAND_MAX;
				}


				sphereVertices[idx].push_back(glm::vec3(xPos, yPos, zPos));
				// std::cout << xPos << " " << yPos << " " << zPos << std::endl;
			}
		}
	}
	void genfaces(int idx) {
		//生成球的面
		for (int i = 0; i < Y_SEGMENTS; i++) {
			for (int j = 0; j < X_SEGMENTS; j++) {
				glm::vec3 v1 = sphereVertices[idx][i * (X_SEGMENTS + 1) + j];
				glm::vec3 v2 = sphereVertices[idx][(i + 1) * (X_SEGMENTS + 1) + j];
				glm::vec3 v3 = sphereVertices[idx][(i + 1) * (X_SEGMENTS + 1) + j + 1];

				glm::vec3 norm = glm::cross(v3 - v1, v2 - v1);
				glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
				// glm::vec3 norm = glm::vec3(1.0f, 0.0f, 0.0f);
				spherefaces[idx].push_back(v1);
				spherefaces[idx].push_back(norm);
				spherefaces[idx].push_back(color);
				spherefaces[idx].push_back(v2);
				spherefaces[idx].push_back(norm);
				spherefaces[idx].push_back(color);
				spherefaces[idx].push_back(v3);
				spherefaces[idx].push_back(norm);
				spherefaces[idx].push_back(color);

				v1 = sphereVertices[idx][i * (X_SEGMENTS + 1) + j];
				v2 = sphereVertices[idx][(i + 1) * (X_SEGMENTS + 1) + j + 1];
				v3 = sphereVertices[idx][i * (X_SEGMENTS + 1) + j + 1];
				norm = glm::cross(v3 - v1, v2 - v1);
				color = glm::vec3(1.0f, 1.0f, 1.0f);
				spherefaces[idx].push_back(v1);
				spherefaces[idx].push_back(norm);
				spherefaces[idx].push_back(color);
				spherefaces[idx].push_back(v2);
				spherefaces[idx].push_back(norm);
				spherefaces[idx].push_back(color);
				spherefaces[idx].push_back(v3);
				spherefaces[idx].push_back(norm);
				spherefaces[idx].push_back(color);
			}
		}

		glGenVertexArrays(1, &this->cloudVAOs[idx]);
		glGenBuffers(1, &this->cloudVBOs[idx]);

		//// bind the Vertex Array Object first, then bind and set vertex buffer(s),
		/// and then configure vertex attributes(s).
		glBindVertexArray(this->cloudVAOs[idx]);
		glBindBuffer(GL_ARRAY_BUFFER, this->cloudVBOs[idx]);
		glBufferData(GL_ARRAY_BUFFER, this->spherefaces[idx].size() * sizeof(glm::vec3),
			&this->spherefaces[idx][0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void*)sizeof(glm::vec3));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void*)(2 * sizeof(glm::vec3)));
		glEnableVertexAttribArray(2);

		sphereVertices[idx].clear();
		std::vector<glm::vec3>().swap(sphereVertices[idx]);
		// 解除绑定
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void generate() {
		this->radius[0] = 1.0f;
		this->radius[1] = 0.6 + 0.25 * std::rand() / RAND_MAX;
		this->radius[2] = 0.6 + 0.25 * std::rand() / RAND_MAX;

		this->internalPositions[0] = glm::vec3(0.0f, 0.0f, 0.0f);
		this->internalPositions[1] = glm::vec3(0.6 + 0.3 * radius[1] * std::rand() / RAND_MAX, 0.0f, 0.0f);
		this->internalPositions[2] = glm::vec3(-0.6f - 0.3 * radius[2] * std::rand() / RAND_MAX, 0.0f, 0.0f);

		for (int i = 0; i < SPHERE_COUNT; i++) this->genVertices(i), this->genfaces(i);
	}



	void deAllocate() {

	}
};

#endif