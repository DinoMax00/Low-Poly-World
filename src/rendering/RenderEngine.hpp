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
#include "ParticleRenderer.hpp"
#include "../animation/bone.h"

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
static BoneAnimation* little_people;
/**
 * 处理gl初始化以及所有渲染事件.
 */
class RenderEngine {
private:
	GLFWwindow* window;
	Fbo* reflection_fbo = 0; // 反射
	Fbo* refraction_fbo = 0; // 折射

	
	float** height_map;

public:

	void set_height_map(float** h) {
		height_map = h;
	}

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

		little_people = new BoneAnimation("resources/model.dae", "resources/diffuse.png", "shaders/bone.vs", "shaders/bone.fs");
		Shader bone_shader("shaders/bone.vs", "shaders/bone.fs");
		little_people->initial(bone_shader, *camera, window);
	}

	~RenderEngine() {
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
	void renderObjs(TerrainRender* terrain, WaterRender* water, ParticleRenderer* particles, Skybox* skybox, CloudRenderer* cloud, Light* light) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_CLIP_DISTANCE0);
		prepareReflection(terrain, light);
		prepareRefraction(terrain, light);
		glDisable(GL_CLIP_DISTANCE0);

		prepare();
		// 先画天空盒
		skybox->draw(camera);

		terrain->render(camera, light, glm::vec4(0, 0, 0, 0));

		water->render(camera, light, 
			reflection_fbo->getColorBuffer(), 
			refraction_fbo->getColorBuffer(),
			refraction_fbo->getDepthBuffer()
		);
		
		camera->ModelPosition.x = std::min(camera->ModelPosition.x, (float)MAP_SIZE * 3 - 5);
		camera->ModelPosition.x = std::max(camera->ModelPosition.x, (float)0);
		camera->ModelPosition.z = std::min(camera->ModelPosition.z, (float)MAP_SIZE * 3 - 5);
		camera->ModelPosition.z = std::max(camera->ModelPosition.z, (float)0);
		Shader bone_shader("shaders/bone.vs", "shaders/bone.fs");
		
		little_people->show(bone_shader, *camera, light->get_direction(), window, get_height(camera->ModelPosition.x, camera->ModelPosition.z));

		particles->render(camera, light->get_direction(), light->get_color());
		cloud->render(camera, light->get_direction(), light->get_color());
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

	float get_height(float z, float x) {
		int r1 = x / 3, c1 = z / 3;
		int r2 = r1, r3 = r1 + 1, r4 = r1 + 1;
		int c2 = c1 + 1, c3 = c1, c4 = c1 + 1;
		glm::vec3 p1, p2, p3;

		if (r1 % 2 != c1 % 2) {
			p1 = glm::vec3(r1 * 3, c1 * 3, 3 * height_map[r1][c1]);
			p2 = glm::vec3(r4 * 3, c4 * 3, 3 * height_map[r4][c4]);
			if (x - r1 * 3 < z - c1 * 3) 
				p3 = glm::vec3(r2 * 3, c2 * 3, 3 * height_map[r2][c2]);
			else 
				p3 = glm::vec3(r3 * 3, c3 * 3, 3 * height_map[r3][c3]);
		}
		else {
			p1 = glm::vec3(r2 * 3, c2 * 3, 3 * height_map[r2][c2]);
			p2 = glm::vec3(r3 * 3, c3 * 3, 3 * height_map[r3][c3]);
			if (x - r1 * 3 < c2 * 3 - z)
				p3 = glm::vec3(r1 * 3, c1 * 3, 3 * height_map[r1][c1]);
			else
				p3 = glm::vec3(r4 * 3, c4 * 3, 3 * height_map[r4][c4]);
		}

		p1 -= p3;
		p2 -= p3;
		auto p = glm::vec3(p1.y * p2.z - p1.z * p2.y, -p1.x * p2.z + p1.z * p2.x, p1.x * p2.y - p1.y * p2.x);
		auto d = p.x * p3.x + p.y * p3.y + p.z * p3.z;
		float ans = d - p.x * x - p.y * z;
		ans /= p.z;
		return ans;
	}
};

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		camera->free_view = little_people->isstop = false;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->ProcessKeyboard(FORWARD, 20 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(BACKWARD, 20 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(LEFT, 20 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(RIGHT, 20 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera->free_view = little_people->isstop =  true;
	
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