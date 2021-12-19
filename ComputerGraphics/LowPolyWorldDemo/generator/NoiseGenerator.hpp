#pragma once

#include <stdlib.h>
#include <iostream>
#include <ctime>

#include "../config.h"

/**
 * @brief 柏林噪声生成器
 */

class NoiseGenerator {
private:
	int seed;
	float roughness;
	int octaves;
	float amplitude;

public:
	/**
	 * @brief 无参构造 参数先直接从config里取了
	 *
	 */
	NoiseGenerator(float _roughness, int _octaves, float _amplitude): roughness(_roughness), octaves(_octaves), amplitude(_amplitude) {
		seed = (int)time(0);
	}

	/**
	 * @brief 获取二维坐标的柏林噪声
	 * @param [x,y] 整点坐标
	 *
	 * @return 浮点随机高度
	 */
	float getPerlinNoise(int x, int y) {
		float tot = 0;
		float d = (float)std::pow(2, octaves - 1);
		for (int i = 0; i < octaves; ++i) {
			float freq = (float)std::pow(2, i) / d;
			float amp = (float)std::pow(roughness, i) * amplitude;
			tot += getInterpolateNoise(x * freq, y * freq) * amp;
		}
		//std::cout << tot << std::endl;
		return tot;
	}

private:
	float getNoise(int x, int y) {

		srand(x * 1145 * std::pow(2, x % 13) + y * 14114 * (int)std::pow(2, y % 7) + seed);
		auto rd = rand();
 		return (1.0f * rd / RAND_MAX) * 2.0f - 1.0f;
	}

	float interpolate(float a, float b, float blend) {
		double theta = blend * std::acos(-1);
		float f = (float)((1.0f - std::cos(theta)) * 0.5f);
		return a * (1 - f) + b * f;
	}

	float getSmoothNoise(int x, int y) {
		float corners = (getNoise(x - 1, y - 1) + getNoise(x + 1, y - 1) + getNoise(x - 1, y + 1)+ getNoise(x + 1, y + 1)) / 16.0f;
		float sides = (getNoise(x - 1, y) + getNoise(x + 1, y) + getNoise(x, y - 1) + getNoise(x, y + 1)) / 8.0f;
		float center = getNoise(x, y) / 4.0f;
		return corners + sides + center;
	}

	float getInterpolateNoise(float x, float y) {
		int intX = (int)x;
		float fracX = x - intX;
		int intY = (int)y;
		float fracY = y - intY;

		float v1 = getSmoothNoise(intX, intY);
		float v2 = getSmoothNoise(intX + 1, intY);
		float v3 = getSmoothNoise(intX, intY + 1);
		float v4 = getSmoothNoise(intX + 1, intY + 1);
		float i1 = interpolate(v1, v2, fracX);
		float i2 = interpolate(v3, v4, fracX);
		return interpolate(i1, i2, fracY);
	}
};