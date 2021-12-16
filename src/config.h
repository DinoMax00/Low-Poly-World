#pragma once

/// ���ڴ�С
constexpr unsigned int WINDOW_H = 720;
constexpr unsigned int WINDOW_W = 1280;

/// ���
constexpr float NEAR_PLANE = 0.4f;
constexpr float FAR_PLANE = 2500;

/// ��ͼ��Ϣ
constexpr unsigned int MAP_SIZE = 50;

/// ��������
constexpr float ROUGHNESS = 0.45f; // ƽ���̶�
constexpr int OCTAVES = 3;

/// �����������
constexpr float AMPLITUDE = 15; // ���߶�

/// ˮ�������
constexpr float WATER_HEIGHT = -1.0;
constexpr float WAVE_SPEED = 0.002f;
constexpr float REFRACT_OFFSET = 1.0f;
constexpr float REFLECT_OFFSET = 0.1f;

// �����
constexpr float CLOUD_SCALE = 2.5f; // ���ڵ�����С
constexpr float CLOUD_BASE_HEIGHT = AMPLITUDE + 6; // �����߶� ���ڸ߶�����ƫ��
constexpr int SEGMENT_COUNT = 15;
constexpr float PI = 3.14159265358979323846f;

/// �������
constexpr glm::vec3 LIGHT_DIRECTION = glm::vec3(0.3f, -0.1f, 0.5f);
constexpr glm::vec3 LIGHT_COLOR = glm::vec3(1.0f, 1.0f, 1.0f);
constexpr glm::vec2 LIGHT_BIAS = glm::vec2(0.3f, 0.8f);

/// ��ɫ�������
constexpr float COLOR_GEN_SPREAD = 0.45f;
static const float COLOR_GEN_ARR[][3] = {
	{ 201.0 / 255.0, 178.0 / 255.0, 99.0 / 255.0 },
	{ 135.0 / 255.0, 184.0 / 255.0, 82.0 / 255.0 },
	{ 80.0 / 255.0, 171.0 / 255.0, 93.0 / 255.0 },
	{ 120.0 / 255.0, 120.0 / 255.0, 120.0 / 255.0 },
	{ 200.0 / 255.0, 200.0 / 255.0, 210.0 / 255.0 },
};
