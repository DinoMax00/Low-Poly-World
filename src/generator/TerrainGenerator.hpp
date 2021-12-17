#pragma once

#include "NoiseGenerator.hpp"
#include "ColourGenerator.hpp"
#include "../objects/GridSquare.hpp"
#include "../rendering/TerrainRender.hpp"
#include "../config.h"

#include "AquNoise.hpp"

/**
 * 地形生成
 */
class TerrainGenerator {
private:
	NoiseGenerator* snow;
	ColorGenerator* snow_color;
	
	float** height_map;
	color** color_map;

	ColorGenerator* stone_color;
	NoiseGenerator* stone;
	
	ColorGenerator* sand_color;
	NoiseGenerator* sand;
	
	ColorGenerator* plain_color;
	AquNoise* plain;

public:
	/**
	 * 无参构造
	 */
	TerrainGenerator() {

		stone = new NoiseGenerator(ROUGHNESS, OCTAVES, AMPLITUDE1);
		stone_color = new ColorGenerator(COLOR_ARR_LEN1, COLOR_ARR1, AMPLITUDE1);

		snow = new NoiseGenerator(ROUGHNESS, OCTAVES + 2, AMPLITUDE2);
		snow_color = new ColorGenerator(COLOR_ARR_LEN5, COLOR_ARR5, AMPLITUDE2);

		sand = new NoiseGenerator(ROUGHNESS / 2, OCTAVES + 2, AMPLITUDE2);
		sand_color = new ColorGenerator(COLOR_ARR_LEN4, COLOR_ARR4, AMPLITUDE2);

		plain = new AquNoise();
		plain_color = new ColorGenerator(COLOR_ARR_LEN6, COLOR_ARR6, AMPLITUDE1);

		color** tmp_color_map = new color * [MAP_SIZE + 1];
		for (int z = 0; z < MAP_SIZE + 1; ++z)
			tmp_color_map[z] = new color[MAP_SIZE + 1];
		float** tmp_height_map = new float* [MAP_SIZE + 1];
		for (int z = 0; z < MAP_SIZE + 1; ++z)
			tmp_height_map[z] = new float[MAP_SIZE + 1];
		float** part_height_map[4];
		part_height_map[0] = new float* [MAP_SIZE + 1];
		part_height_map[1] = new float* [MAP_SIZE + 1];
		part_height_map[2] = new float* [MAP_SIZE + 1];
		part_height_map[3] = new float* [MAP_SIZE + 1];
		for (int z = 0; z < MAP_SIZE + 1; ++z)
			part_height_map[0][z] = new float[MAP_SIZE + 1],
			part_height_map[1][z] = new float[MAP_SIZE + 1],
			part_height_map[2][z] = new float[MAP_SIZE + 1],
			part_height_map[3][z] = new float[MAP_SIZE + 1];
		color** part_color_map[4];
		part_color_map[0] = new color * [MAP_SIZE + 1];
		part_color_map[1] = new color * [MAP_SIZE + 1];
		part_color_map[2] = new color * [MAP_SIZE + 1];
		part_color_map[3] = new color * [MAP_SIZE + 1];
		for (int z = 0; z < MAP_SIZE + 1; ++z)
			part_color_map[0][z] = new color[MAP_SIZE + 1],
			part_color_map[1][z] = new color[MAP_SIZE + 1],
			part_color_map[2][z] = new color[MAP_SIZE + 1],
			part_color_map[3][z] = new color[MAP_SIZE + 1];

		generateHeightMap(1);
		generateColorMap(1);
		for (int z = 0; z < MAP_SIZE + 1; ++z)
		{
			for (int x = 0; x < MAP_SIZE + 1; ++x)
			{
				part_height_map[0][z][x] = height_map[z][x];
				part_color_map[0][z][x] = color_map[z][x];
			}
		}

		generateHeightMap(2);
		generateColorMap(2);
		for (int z = 0; z < MAP_SIZE + 1; ++z)
		{
			for (int x = 0; x < MAP_SIZE + 1; ++x)
			{
				part_height_map[1][z][x] = height_map[z][x];
				part_color_map[1][z][x] = color_map[z][x];
			}
		}
		generateHeightMap(3);
		generateColorMap(3);
		for (int z = 0; z < MAP_SIZE + 1; ++z)
		{
			for (int x = 0; x < MAP_SIZE + 1; ++x)
			{
				part_height_map[2][z][x] = height_map[z][x];
				part_color_map[2][z][x] = color_map[z][x];
			}
		}
		generateHeightMap(4);
		generateColorMap(4);
		for (int z = 0; z < MAP_SIZE + 1; ++z)
		{
			for (int x = 0; x < MAP_SIZE + 1; ++x)
			{
				part_height_map[3][z][x] = height_map[z][x];
				part_color_map[3][z][x] = color_map[z][x];
			}
		}

		for (int z = 0; z < MAP_SIZE + 1; ++z)
		{
			for (int x = 0; x < MAP_SIZE + 1; ++x)
			{
				if (x <= MAP_SIZE / 2 && z <= MAP_SIZE / 2)
					height_map[z][x] = part_height_map[0][z][x],
					color_map[z][x] = part_color_map[0][z][x];
				if (x > MAP_SIZE / 2 && z <= MAP_SIZE / 2)
					height_map[z][x] = part_height_map[2][z][x],
					color_map[z][x] = part_color_map[2][z][x];
				if (x <= MAP_SIZE / 2 && z > MAP_SIZE / 2)
					height_map[z][x] = part_height_map[1][z][x],
					color_map[z][x] = part_color_map[1][z][x];
				if (x > MAP_SIZE / 2 && z > MAP_SIZE / 2)
					height_map[z][x] = part_height_map[3][z][x],
					color_map[z][x] = part_color_map[3][z][x];
			}
		}
		for (int z = 0; z < MAP_SIZE + 1; ++z)
		{
			for (int x = 0; x < MAP_SIZE + 1; ++x)
			{
				tmp_height_map[z][x] = height_map[z][x];
				tmp_color_map[z][x] = color_map[z][x];
			}
		}


		for (int z = 1; z < MAP_SIZE / 4; z++)
		{
			for (int x = 0; x < MAP_SIZE / 2; ++x)
			{
				double pos = part_height_map[0][MAP_SIZE / 2 - z][x] * (double(z) * 2 / MAP_SIZE + 0.5) + part_height_map[1][MAP_SIZE / 2 - z][x] * (double(-z) * 2 / MAP_SIZE + 0.5);
				double neg = part_height_map[0][MAP_SIZE / 2 + z][x] * (double(-z) * 2 / MAP_SIZE + 0.5) + part_height_map[1][MAP_SIZE / 2 + z][x] * (double(z) * 2 / MAP_SIZE + 0.5);
				glm::vec3 pos_c = part_color_map[0][MAP_SIZE / 2 - z][x] * (float)(double(z) * 2 / MAP_SIZE + 0.5) + part_color_map[1][MAP_SIZE / 2 - z][x] * (float)(double(-z) * 2 / MAP_SIZE + 0.5);
				glm::vec3 neg_c = part_color_map[0][MAP_SIZE / 2 + z][x] * (float)(double(-z) * 2 / MAP_SIZE + 0.5) + part_color_map[1][MAP_SIZE / 2 + z][x] * (float)(double(z) * 2 / MAP_SIZE + 0.5);
				height_map[MAP_SIZE / 2 - z][x] = pos;
				height_map[MAP_SIZE / 2 + z][x] = neg;
				color_map[MAP_SIZE / 2 - z][x] = pos_c;
				color_map[MAP_SIZE / 2 + z][x] = neg_c;
			}
			for (int x = MAP_SIZE / 2; x < MAP_SIZE + 1; ++x)
			{
				double pos = part_height_map[2][MAP_SIZE / 2 - z][x] * (double(z) * 2 / MAP_SIZE + 0.5) + part_height_map[3][MAP_SIZE / 2 - z][x] * (double(-z) * 2 / MAP_SIZE + 0.5);
				double neg = part_height_map[2][MAP_SIZE / 2 + z][x] * (double(-z) * 2 / MAP_SIZE + 0.5) + part_height_map[3][MAP_SIZE / 2 + z][x] * (double(z) * 2 / MAP_SIZE + 0.5);
				glm::vec3 pos_c = part_color_map[2][MAP_SIZE / 2 - z][x] * (float)(double(z) * 2 / MAP_SIZE + 0.5) + part_color_map[3][MAP_SIZE / 2 - z][x] * (float)(double(-z) * 2 / MAP_SIZE + 0.5);
				glm::vec3 neg_c = part_color_map[2][MAP_SIZE / 2 + z][x] * (float)(double(-z) * 2 / MAP_SIZE + 0.5) + part_color_map[3][MAP_SIZE / 2 + z][x] * (float)(double(z) * 2 / MAP_SIZE + 0.5);
				height_map[MAP_SIZE / 2 - z][x] = pos;
				height_map[MAP_SIZE / 2 + z][x] = neg;
				color_map[MAP_SIZE / 2 - z][x] = pos_c;
				color_map[MAP_SIZE / 2 + z][x] = neg_c;
			}
			for (int x = 0; x < MAP_SIZE + 1; ++x)
				height_map[MAP_SIZE / 2][x] = (height_map[MAP_SIZE / 2 + 1][x] + height_map[MAP_SIZE / 2 - 1][x]) / 2.0,
				color_map[MAP_SIZE / 2][x] = (color_map[MAP_SIZE / 2 + 1][x] + color_map[MAP_SIZE / 2 - 1][x]) / (float)2.0;
		}

		for (int x = 1; x < MAP_SIZE / 4; x++)
		{
			for (int z = 0; z < MAP_SIZE / 2; ++z)
			{
				double pos = part_height_map[0][z][MAP_SIZE / 2 - x] * (double(x) * 2 / MAP_SIZE + 0.5) + part_height_map[2][z][MAP_SIZE / 2 - x] * (double(-x) * 2 / MAP_SIZE + 0.5);
				double neg = part_height_map[0][z][MAP_SIZE / 2 + x] * (double(-x) * 2 / MAP_SIZE + 0.5) + part_height_map[2][z][MAP_SIZE / 2 + x] * (double(x) * 2 / MAP_SIZE + 0.5);
				glm::vec3 pos_c = part_color_map[0][z][MAP_SIZE / 2 - x] * (float)(double(x) * 2 / MAP_SIZE + 0.5) + part_color_map[2][z][MAP_SIZE / 2 - x] * (float)(double(-x) * 2 / MAP_SIZE + 0.5);
				glm::vec3 neg_c = part_color_map[0][z][MAP_SIZE / 2 + x] * (float)(double(-x) * 2 / MAP_SIZE + 0.5) + part_color_map[2][z][MAP_SIZE / 2 + x] * (float)(double(x) * 2 / MAP_SIZE + 0.5);
				tmp_height_map[z][MAP_SIZE / 2 - x] = pos;
				tmp_height_map[z][MAP_SIZE / 2 + x] = neg;
				tmp_color_map[z][MAP_SIZE / 2 - x] = pos_c;
				tmp_color_map[z][MAP_SIZE / 2 + x] = neg_c;
			}
			for (int z = MAP_SIZE / 2; z < MAP_SIZE + 1; ++z)
			{
				double pos = part_height_map[1][z][MAP_SIZE / 2 - x] * (double(x) * 2 / MAP_SIZE + 0.5) + part_height_map[3][z][MAP_SIZE / 2 - x] * (double(-x) * 2 / MAP_SIZE + 0.5);
				double neg = part_height_map[1][z][MAP_SIZE / 2 + x] * (double(-x) * 2 / MAP_SIZE + 0.5) + part_height_map[3][z][MAP_SIZE / 2 + x] * (double(x) * 2 / MAP_SIZE + 0.5);
				glm::vec3 pos_c = part_color_map[1][z][MAP_SIZE / 2 - x] * (float)(double(x) * 2 / MAP_SIZE + 0.5) + part_color_map[3][z][MAP_SIZE / 2 - x] * (float)(double(-x) * 2 / MAP_SIZE + 0.5);
				glm::vec3 neg_c = part_color_map[1][z][MAP_SIZE / 2 + x] * (float)(double(-x) * 2 / MAP_SIZE + 0.5) + part_color_map[3][z][MAP_SIZE / 2 + x] * (float)(double(x) * 2 / MAP_SIZE + 0.5);
				tmp_height_map[z][MAP_SIZE / 2 - x] = pos;
				tmp_height_map[z][MAP_SIZE / 2 + x] = neg;
				tmp_color_map[z][MAP_SIZE / 2 - x] = pos_c;
				tmp_color_map[z][MAP_SIZE / 2 + x] = neg_c;
			}
			for (int z = 0; z < MAP_SIZE + 1; ++z)
				tmp_height_map[z][MAP_SIZE / 2] = (tmp_height_map[z][MAP_SIZE / 2 + 1] + tmp_height_map[z][MAP_SIZE / 2 - 1]) / 2.0,
				tmp_color_map[z][MAP_SIZE / 2] = (tmp_color_map[z][MAP_SIZE / 2 + 1] + tmp_color_map[z][MAP_SIZE / 2 - 1]) / (float)2.0;
		}

		for (int z = 0; z < MAP_SIZE + 1; ++z)
		{
			for (int x = 0; x < MAP_SIZE + 1; ++x)
			{
				height_map[z][x] = (height_map[z][x] + tmp_height_map[z][x]) / 2.0;
				color_map[z][x] = (color_map[z][x] + tmp_color_map[z][x]) / (float)2.0;
			}
		}
		for (int z = 1; z < MAP_SIZE / 4; z++)
		{
			for (int x = 0; x < MAP_SIZE + 1; ++x)
			{
				double pos = height_map[MAP_SIZE / 2 - z][x] * (double(z) * 2 / MAP_SIZE + 0.5) + height_map[MAP_SIZE / 2 + z][x] * (double(-z) * 2 / MAP_SIZE + 0.5);
				double neg = height_map[MAP_SIZE / 2 - z][x] * (double(-z) * 2 / MAP_SIZE + 0.5) + height_map[MAP_SIZE / 2 + z][x] * (double(z) * 2 / MAP_SIZE + 0.5);
				glm::vec3 pos_c = color_map[MAP_SIZE / 2 - z][x] * (float)(double(z) * 2 / MAP_SIZE + 0.5) + color_map[MAP_SIZE / 2 + z][x] * (float)(double(-z) * 2 / MAP_SIZE + 0.5);
				glm::vec3 neg_c = color_map[MAP_SIZE / 2 - z][x] * (float)(double(-z) * 2 / MAP_SIZE + 0.5) + color_map[MAP_SIZE / 2 + z][x] * (float)(double(z) * 2 / MAP_SIZE + 0.5);
				height_map[MAP_SIZE / 2 - z][x] = pos;
				height_map[MAP_SIZE / 2 + z][x] = neg;
				color_map[MAP_SIZE / 2 - z][x] = pos_c;
				color_map[MAP_SIZE / 2 + z][x] = neg_c;
			}
			for (int x = 0; x < MAP_SIZE + 1; ++x)
				height_map[MAP_SIZE / 2][x] = (height_map[MAP_SIZE / 2 + 1][x] + height_map[MAP_SIZE / 2 - 1][x]) / 2.0,
				color_map[MAP_SIZE / 2][x] = (color_map[MAP_SIZE / 2 + 1][x] + color_map[MAP_SIZE / 2 - 1][x]) / (float)2.0;
		}
		for (int x = 1; x < MAP_SIZE / 4; x++)
		{
			for (int z = 0; z < MAP_SIZE + 1; ++z)
			{
				double pos = height_map[z][MAP_SIZE / 2 - x] * (double(x) * 2 / MAP_SIZE + 0.5) + height_map[z][MAP_SIZE / 2 + x] * (double(-x) * 2 / MAP_SIZE + 0.5);
				double neg = height_map[z][MAP_SIZE / 2 - x] * (double(-x) * 2 / MAP_SIZE + 0.5) + height_map[z][MAP_SIZE / 2 + x] * (double(x) * 2 / MAP_SIZE + 0.5);
				glm::vec3 pos_c = color_map[z][MAP_SIZE / 2 - x] * (float)(double(x) * 2 / MAP_SIZE + 0.5) + color_map[z][MAP_SIZE / 2 + x] * (float)(double(-x) * 2 / MAP_SIZE + 0.5);
				glm::vec3 neg_c = color_map[z][MAP_SIZE / 2 - x] * (float)(double(-x) * 2 / MAP_SIZE + 0.5) + color_map[z][MAP_SIZE / 2 + x] * (float)(double(x) * 2 / MAP_SIZE + 0.5);
				height_map[z][MAP_SIZE / 2 - x] = pos;
				height_map[z][MAP_SIZE / 2 + x] = neg;
				color_map[z][MAP_SIZE / 2 - x] = pos_c;
				color_map[z][MAP_SIZE / 2 + x] = neg_c;
			}
			for (int z = 0; z < MAP_SIZE + 1; ++z)
				height_map[z][MAP_SIZE / 2] = (height_map[z][MAP_SIZE / 2 + 1] + height_map[z][MAP_SIZE / 2 - 1]) / 2.0,
				color_map[z][MAP_SIZE / 2] = (color_map[z][MAP_SIZE / 2 + 1] + color_map[z][MAP_SIZE / 2 - 1]) / (float)2.0;
		}
	}

	/**
	 * 生成一个Terrain类.
	 */
	TerrainRender* createTerrain() {
		int vertex_count = calculateVertexCount(MAP_SIZE + 1);
		auto buffer = createMeshData(vertex_count);
		auto indices = generateIdxBuffer(MAP_SIZE + 1);
		int buffer_len = vertex_count;
		int idc_len = MAP_SIZE * MAP_SIZE * 6;

		return new TerrainRender(buffer_len, buffer, idc_len, indices);
	}

private:

	/**
	 * 根据柏林噪声生成地形高度图
	 */
	void generateHeightMap(int noise_num) {
		height_map = new float* [MAP_SIZE + 1];
		// 生成的地图在x-z平面上
		for (int z = 0; z < MAP_SIZE + 1; ++z) {
			height_map[z] = new float[MAP_SIZE + 1];
			for (int x = 0; x < MAP_SIZE + 1; ++x) {
				if (noise_num == 1) height_map[z][x] = snow->getPerlinNoise(x, z) + 2.5;
				else if (noise_num == 2) height_map[z][x] = stone->getPerlinNoise(x, z);
				else if (noise_num == 3) height_map[z][x] = plain->noiseLayer(1.0 * z / 100, 1.0 * x / 100) + 4;
				else if (noise_num == 4) height_map[z][x] = sand->getPerlinNoise(x, z) + 10;
			}
		}
	}

	/**
	 * 生成地形图后，再生成各点对应的颜色.
	 *
	 */
	void generateColorMap(int noise_num) {
		color_map = new color * [MAP_SIZE + 1];
		// 生成的地图在x-z平面上
		for (int z = 0; z < MAP_SIZE + 1; ++z) {
			color_map[z] = new color[MAP_SIZE + 1];
			for (int x = 0; x < MAP_SIZE + 1; ++x) {
				if (noise_num == 1)color_map[z][x] = snow_color->generate(height_map[z][x]);
				else if (noise_num == 2)color_map[z][x] = stone_color->generate(height_map[z][x]);
				else if (noise_num == 3)color_map[z][x] = plain_color->generate(height_map[z][x]);
				else if (noise_num == 4)color_map[z][x] = sand_color->generate(height_map[z][x]);
			}
		}
	}

	/**
	 * 计算总顶点数.
	 *
	 * \param vertexLength
	 * \return
	 */
	int calculateVertexCount(int vertexLength)
	{
		int bottom2Rows = 2 * vertexLength;
		int remainingRowCount = vertexLength - 2;
		int topCount = remainingRowCount * (vertexLength - 1) * 2;
		return (topCount + bottom2Rows) * 3;
	}

	/**
	 * 储存所有顶点信息.
	 *
	 * \param vertex_count
	 */
	glm::vec3* createMeshData(int vertex_count)
	{
		int idx = 0;
		auto buffer = new glm::vec3[vertex_count];
		auto last_row = new GridSquare[MAP_SIZE + 1];
		for (int row = 0; row < MAP_SIZE; row++)
		{
			for (int col = 0; col < MAP_SIZE; col++)
			{
				GridSquare square(row, col, height_map, color_map);
				square.storeSquareData(buffer, idx);
				if (row == MAP_SIZE - 1)
				{
					last_row[col] = square;
				}
			}
		}

		for (int i = 0; i < MAP_SIZE; i++)
		{
			last_row[i].storeBottomRowData(buffer, idx);
		}

		assert(vertex_count == idx);
		delete[] last_row;
		return buffer;
	}

	/**
	 * 分三部分存储地图的顶点索引信息.
	 *
	 * \param vertex_count 顶点总数
	 * \return 数组地址
	 */
	int* generateIdxBuffer(unsigned int vertex_count) {
		int len = (vertex_count - 1) * (vertex_count - 1) * 6;
		int* indices = new int[len];
		int rowLength = (vertex_count - 1) * 2;
		int pointer = storeTopSection(indices, rowLength, vertex_count);
		pointer = storeSecondLastLine(indices, pointer, rowLength, vertex_count);
		pointer = storeLastLine(indices, pointer, rowLength, vertex_count);

		assert(len == pointer);
		return indices;
	}

	/**
	 * 保存前几排的三角形索引信息.
	 *
	 * \param indices
	 * \param rowLength
	 * \param vertexLength
	 * \return
	 */
	int storeTopSection(int* indices, int rowLength, int vertexLength)
	{
		int pointer = 0;
		for (int row = 0; row < vertexLength - 3; row++)
		{
			for (int col = 0; col < vertexLength - 1; col++)
			{
				// 四个顶点索引
				int topLeft = (row * rowLength) + (col * 2);
				int topRight = topLeft + 1;
				int bottomLeft = topLeft + rowLength;
				int bottomRight = bottomLeft + 1;
				pointer = storeQuad(topLeft, topRight, bottomLeft, bottomRight, indices, pointer, col % 2 != row % 2);
			}
		}
		return pointer;
	}

	/**
	 * 保存倒数第二行三角形信息.
	 *
	 * \param indices
	 * \param pointer
	 * \param rowLength
	 * \param vertexLength
	 * \return
	 */
	int storeSecondLastLine(int* indices, int pointer, int rowLength, int vertexLength)
	{
		int row = vertexLength - 3;
		for (int col = 0; col < vertexLength - 1; col++)
		{
			int topLeft = (row * rowLength) + (col * 2);
			int topRight = topLeft + 1;
			int bottomLeft = (topLeft + rowLength) - col;
			int bottomRight = bottomLeft + 1;
			pointer = storeQuad(topLeft, topRight, bottomLeft, bottomRight, indices, pointer, col % 2 != row % 2);
		}
		return pointer;
	}

	/**
	 * 保存最后一行三角形信息.
	 *
	 * \param indices
	 * \param pointer
	 * \param rowLength
	 * \param vertexLength
	 * \return
	 */
	int storeLastLine(int* indices, int pointer, int rowLength, int vertexLength)
	{
		int row = vertexLength - 2;
		for (int col = 0; col < vertexLength - 1; col++)
		{
			int topLeft = (row * rowLength) + col;
			int topRight = topLeft + 1;
			int bottomLeft = (topLeft + vertexLength);
			int bottomRight = bottomLeft + 1;
			pointer = storeLastRowQuad(topLeft, topRight, bottomLeft, bottomRight, indices, pointer, col % 2 != row % 2);
		}
		return pointer;
	}

	/**
	 * 存储三角形的索引.
	 *
	 * \param topLeft 正方形四个点坐标
	 * \param topRight
	 * \param bottomLeft
	 * \param bottomRight
	 * \param indices 索引数组
	 * \param pointer 三角形索引
	 * \param rightHanded 三角形类型
	 * \return
	 */
	int storeQuad(int topLeft, int topRight, int bottomLeft, int bottomRight, int* indices, int pointer, bool rightHanded)
	{
		pointer = storeLeftTriangle(topLeft, topRight, bottomLeft, bottomRight, indices, pointer, rightHanded);
		indices[pointer++] = topRight;
		indices[pointer++] = rightHanded ? topLeft : bottomLeft;
		indices[pointer++] = bottomRight;
		return pointer;
	}

	int storeLastRowQuad(int topLeft, int topRight, int bottomLeft, int bottomRight, int* indices, int pointer, bool rightHanded)
	{
		pointer = storeLeftTriangle(topLeft, topRight, bottomLeft, bottomRight, indices, pointer, rightHanded);
		indices[pointer++] = bottomRight;
		indices[pointer++] = topRight;
		indices[pointer++] = rightHanded ? topLeft : bottomLeft;
		return pointer;
	}

	static int storeLeftTriangle(int topLeft, int topRight, int bottomLeft, int bottomRight, int* indices, int pointer, bool rightHanded)
	{
		indices[pointer++] = topLeft;
		indices[pointer++] = bottomLeft;
		indices[pointer++] = rightHanded ? bottomRight : topRight;
		return pointer;
	}
};




