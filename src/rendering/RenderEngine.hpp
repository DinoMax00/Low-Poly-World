#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../lib/camera.h"
#include "../config.h"
#include "../objects/Light.hpp"
#include "../Skybox/Skybox.hpp"
#include "CloudRenderer.hpp"
#include "TerrainRender.hpp"
#include "WaterRender.hpp"

static Camera* camera = new Camera(glm::vec3(50.0f, 10.0f, 50.0f));
static float lastX = WINDOW_W / 2.0f;
static float lastY = WINDOW_H / 2.0f;
static bool firstMouse = true;

// timing 
static float deltaTime = 0.0f;
static float lastFrame = 0.0f;

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

/**
 * 处理gl初始化以及所有渲染事件.
 */
class RenderEngine {
private:
	GLFWwindow* window;

public:

	RenderEngine() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(WINDOW_H, WINDOW_W, "Low-Poly-World", NULL, NULL);

		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			exit(-1);
		}

		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			exit(-1);
		}

		glEnable(GL_DEPTH_TEST);
	}

	~RenderEngine() {
		glfwTerminate();
	}

	bool check_window_close() {
		return glfwWindowShouldClose(window);
	}

	/**
	 * 渲染初始化工作，天空盒作为背景在这里绘制.
	 * 
	 * \param skybox
	 */
	void render_prework(Skybox* skybox) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skybox->draw(*camera);
	};

	/**
	 * 渲染云.
	 *
	 * @param cloud
	 * @param light
	 */
	void render_cloud(CloudRenderer* cloud, Light* light) {
		cloud->render(camera, light->get_direction(), light->get_color());
	}

	void render_postwork() {
		glfwSwapBuffers(window);
		glfwPollEvents();
	};

	/**
	 * 渲染基本地形，这个函数只负责渲染地形和水.
	 * 
	 * \param terrain
	 * \param water
	 * \param light
	 */
	void render_base_obj(TerrainRender* terrain, WaterRender* water, Light* light) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		terrain->render(camera, light);
		glDepthMask(GL_FALSE);
		water->render(camera, light);
		glDepthMask(GL_TRUE);
	}
};

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->ProcessKeyboard(FORWARD, 20 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(BACKWARD, 20 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(LEFT, 20 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(RIGHT, 20 * deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->ProcessMouseScroll(yoffset);
}