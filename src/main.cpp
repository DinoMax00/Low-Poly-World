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

#include "lib/stb_image.h"
#include "lib/camera.h"
#include "lib/shader.h"

#include "config.h"
#include "Skybox/Skybox.hpp"
#include "generator/TerrainGenerator.hpp"
#include "generator/WaterGenerator.hpp"
#include "generator/CloudGenerator.hpp"
#include "objects/Light.hpp"
#include "rendering/RenderEngine.hpp"

int main()
{
	RenderEngine* engine = new RenderEngine();

	TerrainGenerator* terrain_generator = new TerrainGenerator();
	auto terrain = terrain_generator->createTerrain();

	WaterGenerator* water_generator = new WaterGenerator();
	auto water = water_generator->generate();

	CloudGenerator* cloud_generator = new CloudGenerator();
	auto cloud = cloud_generator->generate();

	Light* light = new Light(LIGHT_DIRECTION, LIGHT_COLOR, LIGHT_BIAS);

	Skybox* skybox = new Skybox();
	
	while (!engine->check_window_close())
	{
		engine->render_prework(skybox);
		engine->render_cloud(cloud, light);
		engine->render_base_obj(terrain, water, light);
		engine->render_postwork();
	}
	
	return 0;
}
