#pragma once

#include "NoiseGenerator.hpp"
#include "ColourGenerator.hpp"
#include "../objects/GridSquare.hpp"
#include "../rendering/TerrainRender.hpp"
#include "../config.h"

#include "AquNoise.hpp"

/**
 * ��������
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
	 * �޲ι���
	 */
	TerrainGenerator() {
		aq = new AquNoise();

		noise_gen = new NoiseGenerator(ROUGHNESS, OCTAVES, AMPLITUDE);
		color_gen = new ColorGenerator(COLOR_ARR_LEN1, COLOR_ARR1, AMPLITUDE);

		generateHeightMap();
		generateColorMap();
	}

	/**
	 * ����һ��Terrain��.
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
	 * ���ݰ����������ɵ��θ߶�ͼ
	 */
	void generateHeightMap() {
		height_map = new float* [MAP_SIZE + 1];
		// ���ɵĵ�ͼ��x-zƽ����
		for (int z = 0; z < MAP_SIZE + 1; ++z) {
			height_map[z] = new float[MAP_SIZE + 1];
			for (int x = 0; x < MAP_SIZE + 1; ++x) {
				height_map[z][x] = noise_gen->getPerlinNoise(x, z);
				// height_map[z][x] = aq->noiseLayer(1.0 * z / 100, 1.0 * x / 100);
			}
		}
	}

	/**
	 * ���ɵ���ͼ�������ɸ����Ӧ����ɫ.
	 * 
	 */
	void generateColorMap() {
		color_map = new color* [MAP_SIZE + 1];
		// ���ɵĵ�ͼ��x-zƽ����
		for (int z = 0; z < MAP_SIZE + 1; ++z) {
			color_map[z] = new color[MAP_SIZE + 1];
			for (int x = 0; x < MAP_SIZE + 1; ++x) {
				color_map[z][x] = color_gen->generate(height_map[z][x]);
			}
		}
	}

	/**
	 * �����ܶ�����.
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
	 * �������ж�����Ϣ.
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
	 * �������ִ洢��ͼ�Ķ���������Ϣ.
	 * 
	 * \param vertex_count ��������
	 * \return �����ַ
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
	 * ����ǰ���ŵ�������������Ϣ.
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
				// �ĸ���������
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
	 * ���浹���ڶ�����������Ϣ.
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
	 * �������һ����������Ϣ.
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
	 * �洢�����ε�����.
	 * 
	 * \param topLeft �������ĸ�������
	 * \param topRight
	 * \param bottomLeft
	 * \param bottomRight
	 * \param indices ��������
	 * \param pointer ����������
	 * \param rightHanded ����������
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




