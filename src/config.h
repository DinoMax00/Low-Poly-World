#pragma once

/// 窗口大小
constexpr unsigned int WINDOW_H = 800;
constexpr unsigned int WINDOW_W = 600;

/// 相机
constexpr float NEAR_PLANE = 0.4f;
constexpr float FALR_PLANE = 2500;

/// 地图信息
constexpr unsigned int MAP_SIZE = 100;

/// 柏林噪声
constexpr float ROUGHNESS = 0.45f; // 平滑程度
constexpr int OCTAVES = 3;

/// 地形生成相关
constexpr float AMPLITUDE = 15; // 最大高度

/// 水生成相关
constexpr float WATER_HEIGHT = -1.5;
constexpr float WAVE_SPEED = 0.002f;

/// 光照相关
constexpr glm::vec3 LIGHT_DIRECTION = glm::vec3(0.3f, -0.1f, 0.5f);
constexpr glm::vec3 LIGHT_COLOR = glm::vec3(1.0f, 0.8f, 0.8f);
constexpr glm::vec2 LIGHT_BIAS = glm::vec2(0.3f, 0.8f);

/// 颜色生成相关
constexpr float COLOR_GEN_SPREAD = 0.45f;
static const float COLOR_GEN_ARR[][3] = {
	{ 201.0 / 255.0, 178.0 / 255.0, 99.0 / 255.0 },
	{ 135.0 / 255.0, 184.0 / 255.0, 82.0 / 255.0 },
	{ 80.0 / 255.0, 171.0 / 255.0, 93.0 / 255.0 },
	{ 120.0 / 255.0, 120.0 / 255.0, 120.0 / 255.0 },
	{ 200.0 / 255.0, 200.0 / 255.0, 210.0 / 255.0 },
};
