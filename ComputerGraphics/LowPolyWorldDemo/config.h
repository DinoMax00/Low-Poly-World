#pragma once

/// ���ڴ�С
constexpr unsigned int WINDOW_H = 800;
constexpr unsigned int WINDOW_W = 600;

/// ���
constexpr float NEAR_PLANE = 0.4f;
constexpr float FALR_PLANE = 2500;

/// ��ͼ��Ϣ
constexpr unsigned int MAP_SIZE = 100;

/// ��������
constexpr float ROUGHNESS = 0.45f; // ƽ���̶�
constexpr int OCTAVES = 3;

/// �����������
constexpr float AMPLITUDE = 10; // ���߶�

/// ˮ�������
constexpr float WATER_HEIGHT = -1;
constexpr float WAVE_SPEED = 0.002f;

/// ��ɫ�������
constexpr float COLOR_GEN_SPREAD = 0.45f;
static const float COLOR_GEN_ARR[][3] = {
	{ 201.0 / 255.0, 178.0 / 255.0, 99.0 / 255.0 },
	{ 135.0 / 255.0, 184.0 / 255.0, 82.0 / 255.0 },
	{ 80.0 / 255.0, 171.0 / 255.0, 93.0 / 255.0 },
	{ 120.0 / 255.0, 120.0 / 255.0, 120.0 / 255.0 },
	{ 200.0 / 255.0, 200.0 / 255.0, 210.0 / 255.0 },
};
