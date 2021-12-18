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
#include "Skybox/Skybox.hpp"
#include "generator/TerrainGenerator.hpp"
#include "generator/WaterGenerator.hpp"
#include "generator/CloudGenerator.hpp"
#include "objects/Light.hpp"
#include "rendering/RenderEngine.hpp"

#include "rendering/ModelRender.hpp"
#include "rendering/ShadowRender.hpp"

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
	ModelRender* MR;
	ShadowMapRender* SMR;
	MR = new ModelRender();
	SMR = new ShadowMapRender();

	while (!engine->checkWindowClose())
	{
		engine->renderPrework(skybox);
		
		engine->renderObjs(terrain, water, skybox, cloud, light, MR, SMR);
		engine->renderPostwork();
	}

	return 0;
}