#define _CRT_SECURE_NO_WARNINGS
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iomanip>

#include "lib/stb_image.h"
#include "lib/camera.h"
#include "lib/shader.h"

#include "base.h"

#include "ColourGenerater.h"
#include "GridSquare.h"
#include "IndexGenerator.h"
#include "TerrainGenerator.h"

//��ͼ�߳���������1000
#define SIZE 500

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 15.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

//��ͼ��ɫ����������͵㰴�ո�����ɫ���ȵ���ߵ�
glm::vec3 TERRAIN_COLS[5] = { glm::vec3(201.0/255.0, 178.0/255.0, 99.0 / 255.0),glm::vec3(135.0 / 255.0, 184.0 / 255.0, 82.0 / 255.0), glm::vec3(80.0 / 255.0, 171.0 / 255.0, 93.0 / 255.0), 
glm::vec3(120.0 / 255.0, 120.0 / 255.0, 120.0 / 255.0),glm::vec3(200.0 / 255.0, 200.0 / 255.0, 210.0 / 255.0) };
//glm::vec3 TERRAIN_COLS[5] = { glm::vec3(1, 0,0),glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0),glm::vec3(0, 1, 0) };

int main()
{
	freopen("test.out", "w", stdout);
	srand(time(0));
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Low-Poly-World", NULL, NULL);
	
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	Shader ourShader("shaders/test.vs", "shaders/test.fs");

	ColourGenerator colourGen(TERRAIN_COLS,5);
	TerrainGenerator specialGenerator(colourGen);
	specialGenerator.generateTerrain(SIZE);

	specialGenerator.TerrainRender();
	
	//����λ�ñ���Զ���ڵ�ͼ��С����������ɫ�ҷ������������δ֪ԭ���Ƿ��Ƿ������������Ŀǰʹ�ü�ǿ�������տ���
	glm::vec3 lightPos((float)SIZE/2.0, (float)SIZE / 2.0, (float)SIZE *10.0);
	glm::vec3 lightColour(1.0f, 1.0f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		specialGenerator.TerrainUseshader(ourShader, camera, lightPos, lightColour);
		specialGenerator.TerrainDraw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	specialGenerator.TerrainDelete();
	glfwTerminate();
	return 0;
}