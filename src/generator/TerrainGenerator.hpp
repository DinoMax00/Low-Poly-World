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
	NoiseGenerator* noise_gen;
	ColorGenerator* color_gen;
	float** height_map;
	color** color_map;

	AquNoise* aq;

public:
	/**
	 * 无参构造
	 */
	TerrainGenerator() {
		aq = new AquNoise();

		noise_gen = new NoiseGenerator();
		color_gen = new ColorGenerator();
		generateHeightMap();
		color_map = color_gen->generate(height_map);
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
	void generateHeightMap() {
		height_map = new float* [MAP_SIZE + 1];
		// 生成的地图在x-z平面上
		for (int z = 0; z < MAP_SIZE + 1; ++z) {
			height_map[z] = new float[MAP_SIZE + 1];
			for (int x = 0; x < MAP_SIZE + 1; ++x) {
				height_map[z][x] = noise_gen->getPerlinNoise(x, z);
				// height_map[z][x] = aq->noiseLayer(1.0 * z / 100, 1.0 * x / 100);
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




