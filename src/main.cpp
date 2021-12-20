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

// #define STB_IMAGE_IMPLEMENTATION
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
#include "rendering/ParticleRenderer.hpp"
#include "animation/bone.h"

int main()
{
	RenderEngine* engine = new RenderEngine();
	TerrainGenerator* terrain_generator = new TerrainGenerator();
	auto terrain = terrain_generator->createTerrain();

	WaterGenerator* water_generator = new WaterGenerator();
	auto water = water_generator->generate();

	CloudGenerator* cloud_generator = new CloudGenerator();
	auto cloud = cloud_generator->generate();

	ParticleRenderer* particles = new ParticleRenderer();
	particles->createEmitter(glm::vec3(1.0f, 1.0f, 1.0f),
							 glm::vec3(2.0f),
							 glm::vec3(0.0f, -0.16f, 0.0f),
							 glm::vec2(0.0f, 0.0f),
							 glm::vec2(500, 500),
							 AMPLITUDE2 * 2.5 * 2,
							 600 * 5);

	Light* light = new Light(LIGHT_DIRECTION, LIGHT_COLOR, LIGHT_BIAS);

	Skybox* skybox = new Skybox();

	engine->set_height_map(terrain_generator->get_height_map());

	while (!engine->checkWindowClose())
	{
		engine->renderPrework(skybox);
		
		engine->renderObjs(terrain, water, particles, skybox, cloud, light);
		engine->renderPostwork();
	}

	return 0;
}