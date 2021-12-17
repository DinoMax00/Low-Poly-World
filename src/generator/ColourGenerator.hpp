#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <algorithm>

#include "../config.h"

typedef glm::vec3 color;

/**
 * @brief 颜色生成器
 * 根据地形的不同高度产生变化
 */

class ColorGenerator {
private:
	/// 生成颜色根据海拔是渐变的，spread系数决定改变率。
	float spread;
	float half_spread;
	/// 颜色分组信息
	float part;
	/// 颜色生成高度范围
	float amplitude;
	/// 预设的颜色数组，数组内元素由第到高为地形染色。
	color* color_arr;

public:
	/**
	 * 构造函数.
	 * 
	 * \param len 颜色数组长度
	 * \param arr 颜色数组
	 */
	ColorGenerator(int len, color arr[], int _amplitude) {
		amplitude = _amplitude;
		color_arr = arr;
		spread = COLOR_GEN_SPREAD;
		half_spread = spread / 2;

		assert(len != 1);
		part = 1.0f / (len - 1);
	}

	/**
	 * 根据高度生成颜色值.
	 * 
	 * \param height
	 * \return 
	 */
	color generate(float height) {
		return calculateColor(height);
	}

private:

	/**
	 * @brief 根据两颜色计算插值
	 * @param cL,cR 两rgb值
	 * @param lambda 调和系数
	 *
	 * @return 插值
	 */
	color interpolationColor(color cL, color cR, float lambda) {
		float x = 1 - lambda, y = lambda;
		float r = x * cL.r + y * cR.r;
		float g = x * cL.g + y * cR.g;
		float b = x * cL.b + y * cR.b;
		return color(r, g, b);
	}

	/**
	 * @brief 根据高度生成单点颜色信息 
	 * @param height 高度值
	 * 
	 * @return rgb值
	 */
	color calculateColor(float height) {
		float val = (height + amplitude) / (amplitude * 2);
		val = std::max(std::min(.9999f, (val - half_spread) * (1 / spread)), .0f);
		
		// 在颜色数组中的位置
		int pos = (int)std::floor(val / part);

		float lambda = (val - pos * part) / part;
		return interpolationColor(color_arr[pos], color_arr[pos + 1], lambda);
	}
};




