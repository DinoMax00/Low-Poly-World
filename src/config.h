#pragma once

/// 窗口大小
constexpr unsigned int WINDOW_H = 720;
constexpr unsigned int WINDOW_W = 1280;

/// 相机
constexpr float NEAR_PLANE = 0.4f;
constexpr float FAR_PLANE = 2500;
constexpr float LOOK_DIS = 80; // 相机与小人之间的距离

/// 地图信息
constexpr unsigned int MAP_SIZE = 400;

/// 柏林噪声
constexpr float ROUGHNESS = 0.45f; // 平滑程度
constexpr int OCTAVES = 3;

/// 地形生成相关
constexpr float AMPLITUDE1 = 15; // 最大高度
constexpr float AMPLITUDE2 = 30; // 最大高度
constexpr float AMPLITUDE3 = 50; // 最大高度

/// 水生成相关
constexpr float WATER_HEIGHT = -1.0;
constexpr float WAVE_SPEED = 0.002f;
constexpr float REFRACT_OFFSET = 1.0f;
constexpr float REFLECT_OFFSET = 0.1f;

/// 云相关
constexpr int CLOUD_BASE_COUNT = 8; // 基本数目
constexpr float MOVE_SPEED_BASE = 0.050f; // 移动速度
constexpr float CLOUD_SCALE = (float)MAP_SIZE / 10.0; // 用于调整大小
constexpr float CLOUD_BASE_HEIGHT = AMPLITUDE2 * MAP_SIZE / 60.0; // 基础高度 会在高度上下偏置
constexpr int SEGMENT_COUNT = 20;
constexpr float PI = 3.14159265358979323846f;

/// 光照相关
const glm::vec3 LIGHT_DIRECTION = glm::vec3(0.3f, -0.1f, 0.5f);
const glm::vec3 LIGHT_COLOR = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec2 LIGHT_BIAS = glm::vec2(0.3f, 0.8f);

/// 颜色生成相关
constexpr float COLOR_GEN_SPREAD = 0.45f;

// 预置的颜色数组 和数组长度
// 石林
const int COLOR_ARR_LEN1 = 8;
static glm::vec3 COLOR_ARR1[] = {

	{ 201.0 / 255.0, 178.0 / 255.0, 99.0 / 255.0 },
	{	144.0 / 255.0,238.0 / 255.0,144.0 / 255.0},
	{	34.0 / 255.0,139.0 / 255.0,34.0 / 255.0},
	{	0.0 / 255.0,100.0 / 255.0,0.0 / 255.0},
	{	85.0 / 255.0,107.0 / 255.0,47.0 / 255.0},
	{ 120.0 / 255.0, 120.0 / 255.0, 120.0 / 255.0 },
	{	119.0 / 255.0,136.0 / 255.0,153.0 / 255.0},
	{ 	192.0 / 255.0,192.0 / 255.0,192.0 / 255.0 }
};

//山丘
const int COLOR_ARR_LEN2 = 6;
static glm::vec3 COLOR_ARR2[] = {
	{ 201.0 / 255.0, 178.0 / 255.0, 99.0 / 255.0 },
	{ 135.0 / 255.0, 184.0 / 255.0, 82.0 / 255.0 },
	{ 80.0 / 255.0, 171.0 / 255.0, 93.0 / 255.0 },
	{ 143.0 / 255.0 ,188.0 / 255.0    ,143.0 / 255.0 },
	{85.0 / 255.0  ,107.0 / 255.0,  47.0 / 255.0},
	{128.0 / 255.0,128.0 / 255.0,0.0 / 255.0}
	//{	205.0 / 255.0,133.0 / 255.0,63.0/255.0}
};

//火山
const int COLOR_ARR_LEN3 = 5;
static glm::vec3 COLOR_ARR3[] = {
	{ 255.0 / 255.0, 63.0 / 255.0, 0.0 / 255.0 },
	{ 255.0 / 255.0, 79.0 / 255.0, 0.0 / 255.0 },
	{ 134.0 / 255.0, 90.0 / 255.0, 70.0 / 255.0 },
	{ 46.0 / 255.0, 31.0 / 255.0, 25.0 / 255.0 },
	{ 24.0 / 255.0, 22.0 / 255.0, 21.0 / 255.0 }
};

//沙漠
const int COLOR_ARR_LEN4 = 3;
static glm::vec3 COLOR_ARR4[] = {
	{ 181.0 / 255.0, 148.0 / 255.0, 133.0 / 255.0 },
	{ 197.0 / 255.0, 140.0 / 255.0, 42.0 / 255.0 },
	{ 173.0 / 255.0, 119.0 / 255.0, 28.0 / 255.0 }
};

// 雪地
const int COLOR_ARR_LEN5 = 3;
static glm::vec3 COLOR_ARR5[] = {
	{ 255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0 },
	{ 230.0 / 255.0, 230.0 / 255.0, 230.0 / 255.0 },
	{ 217.0 / 255.0, 217.0 / 255.0, 217.0 / 255.0 }
};

// 平原
const int COLOR_ARR_LEN6 = 3;
static glm::vec3 COLOR_ARR6[] = {
	//{ 201.0 / 255.0, 178.0 / 255.0, 99.0 / 255.0 },
	//{ 135.0 / 255.0, 184.0 / 255.0, 82.0 / 255.0 },
	{ 153.0 / 255.0, 253.0 / 255.0, 153.0 / 255.0 },
	{ 0.0 / 255.0, 153 / 255.0, 0.0 / 255.0 },
	{ 0.0 / 255.0, 103.0 / 255.0, 0.0 / 255.0 }
};