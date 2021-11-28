#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:26812)
#pragma warning(disable:26495)
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iomanip>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#endif // !STB_IMAGE_IMPLEMENTATION
#include "lib/camera.h"
#include "lib/shader.h"

#include "config.h"
#include "Skybox/Skybox.h"

#include "generator\TerrainGenerator.hpp"
#include "generator\WaterGenerator.hpp"
#include "rendering/ModelRender.hpp"
// camera
Camera camera(glm::vec3(50.0f, 10.0f, 50.0f));
float lastX = WINDOW_W / 2.0f;
float lastY = WINDOW_H / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, 20 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, 20 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, 20 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, 20 * deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos; 

	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}

int main()
{
	srand((unsigned int)time(0));
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(WINDOW_H, WINDOW_W, "Low-Poly-World", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	TerrainGenerator* terrain_generator = new TerrainGenerator();
	auto terrain = terrain_generator->createTerrain();

	WaterGenerator* water_generator = new WaterGenerator();
	auto water = water_generator->generate();

	//光照位置必须远大于地图大小才有明显颜色且反向比正向亮，未知原因（是否是法向量计算错误？目前使用加强环境光照看清
	// glm::vec3 lightPos((float)MAP_SIZE / 2.0, (float)MAP_SIZE / 2.0, (float)MAP_SIZE * 10.0);
	glm::vec3 lightPos(0.3, -1, 0.5);
	glm::vec3 lightColour(1.0f, 0.8f, 0.8f);

	Skybox skybox;
	skybox.init();

	ModelRender MR;

	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 启动混合并设置混合因子
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		terrain->render(camera, lightPos, lightColour);
		glDepthMask(GL_FALSE);
		water->render(camera, lightPos, lightColour);
		glDepthMask(GL_TRUE);
		skybox.draw(camera);

		{
			glm::mat4 model(1.0f);
			MR.render(camera, shaderTypeOfModel[(int)ModelIndex::TREE1_MAPLE_TREE_0], model, ModelIndex::TREE1_MAPLE_TREE_0);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(20.0f, 0.0f, 20.0f));
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
			MR.render(camera, shaderTypeOfModel[(int)ModelIndex::TREE2_0], model, ModelIndex::TREE2_0);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(50.0f, -5.0f, 50.0f));
			model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
			MR.render(camera, shaderTypeOfModel[(int)ModelIndex::TREE3_1], model, ModelIndex::TREE3_1);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(80.0f, 0.0f, 80.0f));
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			MR.render(camera, shaderTypeOfModel[(int)ModelIndex::TREE4_0], model, ModelIndex::TREE4_0);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(100.0f, 0.0f, 100.0f));
			model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			MR.render(camera, shaderTypeOfModel[(int)ModelIndex::WHITE_TAILED_DEER_1], model, ModelIndex::WHITE_TAILED_DEER_1);

		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}