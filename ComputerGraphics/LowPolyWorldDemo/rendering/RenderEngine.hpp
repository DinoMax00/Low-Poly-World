#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../lib/camera.h"
#include "../config.h"
#include "../objects/Light.hpp"
#include "../Skybox/Skybox.hpp"
#include "../objects/Fbo.hpp"
#include "CloudRenderer.hpp"
#include "TerrainRender.hpp"
#include "WaterRender.hpp"

#include "ModelRender.hpp"
#include "ShadowRender.hpp"

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
	Fbo* reflection_fbo; // 反射
	Fbo* refraction_fbo; // 折射

	Shader* screenShader;
	unsigned int quadVAO, quadVBO;
public:

	RenderEngine() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(WINDOW_W, WINDOW_H, "Low-Poly-World", NULL, NULL);

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
		
		refraction_fbo = new Fbo(WINDOW_W, WINDOW_H, true);
		reflection_fbo = new Fbo(WINDOW_W, WINDOW_H, false);
	}

	~RenderEngine() {
		glDeleteVertexArrays(1, &quadVAO);
		glDeleteBuffers(1, &quadVBO);
		glfwTerminate();
	}

	bool checkWindowClose() {
		return glfwWindowShouldClose(window);
	}

	/**
	 * 渲染初始化工作.
	 * 
	 * \param skybox
	 */
	void renderPrework(Skybox* skybox) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		prepare();
	};

	void renderPostwork() {
		glfwSwapBuffers(window);
		glfwPollEvents();
	};

	/**
	 * 因为某些奇怪的原因，这个函数负责渲染所有东西
	 * 
	 * \param terrain
	 * \param water
	 * \param light
	 */
	void renderObjs(TerrainRender* terrain, WaterRender* water, Skybox* skybox, CloudRenderer* cloud,
		Light* light, ModelRender* MR, ShadowMapRender* SMR)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_CLIP_DISTANCE0);
		prepareReflection(terrain, light);
		prepareRefraction(terrain, light);
		glDisable(GL_CLIP_DISTANCE0);

		prepare();
		// 先画天空盒
		SMR->genShadowMap(*MR, *terrain, *cloud);
		skybox->draw(*camera);
		terrain->render(camera, light, glm::vec4(0, 0, 0, 0), SMR->depthMap, SMR->lightSpaceMatrix);
		//water->render(camera, light, reflection_fbo->getColorBuffer(),
		//	refraction_fbo->getColorBuffer(), refraction_fbo->getDepthBuffer());
		cloud->render(camera, light->get_direction(), light->get_color());
		MR->draw(*camera, SMR->depthMap, SMR->lightSpaceMatrix);
	}

private:

	void prepareReflection(TerrainRender* terrain, Light* light) {
		reflection_fbo->bind();
		camera->reflect();
		prepare();
		terrain->render(camera, light, glm::vec4(0, 1, 0, -WATER_HEIGHT + REFLECT_OFFSET));
		camera->reflect();
		reflection_fbo->unbind();
	}

	void prepareRefraction(TerrainRender* terrain, Light* light) {
		refraction_fbo->bind();
		prepare();
		terrain->render(camera, light, glm::vec4(0, -1, 0, WATER_HEIGHT + REFRACT_OFFSET));
		refraction_fbo->unbind();
	}

	void prepare() {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE); // 抗锯齿
		glEnable(GL_CULL_FACE);   // 面剔除
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