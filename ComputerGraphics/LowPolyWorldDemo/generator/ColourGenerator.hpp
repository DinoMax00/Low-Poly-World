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
	/// ��ɫ���ɸ߶ȷ�Χ
	float amplitude;
	/// Ԥ�����ɫ���飬������Ԫ���ɵڵ���Ϊ����Ⱦɫ��
	color* color_arr;

public:
	/**
	 * ���캯��.
	 * 
	 * \param len ��ɫ���鳤��
	 * \param arr ��ɫ����
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
	 * ���ݸ߶�������ɫֵ.
	 * 
	 * \param height
	 * \return 
	 */
	color generate(float height) {
		return calculateColor(height);
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
		float val = (height + amplitude) / (amplitude * 2);
		val = std::max(std::min(.9999f, (val - half_spread) * (1 / spread)), .0f);
		
		// ����ɫ�����е�λ��
		int pos = (int)std::floor(val / part);

		float lambda = (val - pos * part) / part;
		return interpolationColor(color_arr[pos], color_arr[pos + 1], lambda);
	}
};




