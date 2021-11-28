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
 * @brief ��ɫ������
 * ���ݵ��εĲ�ͬ�߶Ȳ����仯
 */

class ColorGenerator {
private:
	/// ������ɫ���ݺ����ǽ���ģ�spreadϵ�������ı��ʡ�
	float spread;
	float half_spread;
	/// ��ɫ������Ϣ
	float part;

	/// Ԥ�����ɫ���飬������Ԫ���ɵڵ���Ϊ����Ⱦɫ��
	color* color_arr;

public:
	/**
	 * @brief �޲ι���
	 */
	ColorGenerator() {
		int len = sizeof(COLOR_GEN_ARR) / sizeof(COLOR_GEN_ARR[0]);
		color_arr = new color[len];
		for (int i = 0; i < len; ++i) {
			color_arr[i] = color(COLOR_GEN_ARR[i][0], COLOR_GEN_ARR[i][1], COLOR_GEN_ARR[i][2]);
		}
		
		spread = COLOR_GEN_SPREAD;
		half_spread = spread / 2;

		assert(len != 1);
		part = 1.0f / (len - 1);
	}

	/**
	 * @brief ���ݵ�ͼ�߶���Ϣ���ɸ��������ɫ��
	 * @param height_map ��άfloat����
	 * 
	 * @return ��ά����洢�����Ӧrgbֵ
	 */
	color** generate(float** height_map) {
		color** color_map = new color*[MAP_SIZE + 1];

		for (int i = 0; i < MAP_SIZE + 1; ++i) {
			color_map[i] = new color[MAP_SIZE + 1];

			for (int j = 0; j < MAP_SIZE + 1; ++j) {
				color_map[i][j] = calculateColor(height_map[i][j]);
			}
		}

		return color_map;
	}

private:

	/**
	 * @brief ��������ɫ�����ֵ
	 * @param cL,cR ��rgbֵ
	 * @param lambda ����ϵ��
	 * 
	 * @return ��ֵ
	 */
	color interpolationColor(color cL, color cR, float lambda) {
		float x = 1 - lambda, y = lambda;
		float r = x * cL.r + y * cR.r;
		float g = x * cL.g + y * cR.g;
		float b = x * cL.b + y * cR.b;
		return color(r, g, b);
	}

	/**
	 * @brief ���ݸ߶����ɵ�����ɫ��Ϣ 
	 * @param height �߶�ֵ
	 * 
	 * @return rgbֵ
	 */
	color calculateColor(float height) {
		float val = (height + AMPLITUDE) / (AMPLITUDE * 2);
		val = std::max(std::min(.9999f, (val - half_spread) * (1 / spread)), .0f);
		
		// ����ɫ�����е�λ��
		int pos = (int)std::floor(val / part);
		assert(pos < sizeof(COLOR_GEN_ARR) / sizeof(COLOR_GEN_ARR[0]));

		float lambda = (val - pos * part) / part;
		return interpolationColor(color_arr[pos], color_arr[pos + 1], lambda);
	}
};




