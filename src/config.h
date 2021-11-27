#pragma once

/// 窗口大小
constexpr unsigned int WINDOW_H = 800;
constexpr unsigned int WINDOW_W = 600;

/// 地图信息
constexpr unsigned int MAP_SIZE = 100;

/// 柏林噪声
constexpr float ROUGHNESS = 0.45f; // 平滑程度
constexpr int OCTAVES = 3;

/// 地形生成相关
constexpr float AMPLITUDE = 10; // 最大高度

/// 颜色生成相关
constexpr float COLOR_GEN_SPREAD = 0.45f;
static const float COLOR_GEN_ARR[][3] = {
	{ 201.0 / 255.0, 178.0 / 255.0, 99.0 / 255.0 },
	{ 135.0 / 255.0, 184.0 / 255.0, 82.0 / 255.0 },
	{ 80.0 / 255.0, 171.0 / 255.0, 93.0 / 255.0 },
	{ 120.0 / 255.0, 120.0 / 255.0, 120.0 / 255.0 },
	{ 200.0 / 255.0, 200.0 / 255.0, 210.0 / 255.0 },
};
