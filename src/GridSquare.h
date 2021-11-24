#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "base.h"


//方格类，将每个方格分割为两个三角形
class GridSquare {

private:
	int row;
	int col;
	int lastIndex;
	glm::vec3 positions[5];
	glm::vec3 colours[5];
	glm::vec3 normalLeft;
	glm::vec3 normalRight;

public:
	//构造函数
	GridSquare() {}
	GridSquare(int row, int col)
	{
		calculateCornerPositions(col, row);
		calculateCornerColours(col, row);
		this->lastIndex = map.mapSize - 2;
		this->row = row;
		this->col = col;
		bool rightHanded = col % 2 != row % 2;
		this->normalLeft = glm::normalize(glm::cross(positions[rightHanded ? 3 : 2]- positions[0], positions[1] -positions[0]));
		this->normalRight = glm::normalize(glm::cross(positions[3]-positions[2], positions[rightHanded ? 0 : 1] -positions[2]));
	}

	//记录方格信息到顶点数组中
	void storeSquareData()
	{
		map.terrainData[map.dataIndex++] = positions[0];
		map.terrainData[map.dataIndex++] = normalLeft;
		map.terrainData[map.dataIndex++] = colours[0];
		if (row != lastIndex || col == lastIndex)
		{
			map.terrainData[map.dataIndex++] = positions[2];
			map.terrainData[map.dataIndex++] = normalRight;
			map.terrainData[map.dataIndex++] = colours[2];
		}
	}

	//记录最后一行方格信息到顶点数组中
	void storeBottomRowData()
	{
		if (col == 0)
		{
			map.terrainData[map.dataIndex++] = positions[1];
			map.terrainData[map.dataIndex++] = normalLeft;
			map.terrainData[map.dataIndex++] = colours[1];
		}
		map.terrainData[map.dataIndex++] = positions[3];
		map.terrainData[map.dataIndex++] = normalRight;
		map.terrainData[map.dataIndex++] = colours[3];
	}

	//计算顶点颜色，传参为行列
	void calculateCornerColours(int col, int row)
	{
		this->colours[0] = map.mapColour[row][col];
		this->colours[1] = map.mapColour[row + 1][col];
		this->colours[2] = map.mapColour[row][col + 1];
		this->colours[3] = map.mapColour[row + 1][col + 1];
	}

	//计算顶点坐标，传参为行列
	void calculateCornerPositions(int col, int row)
	{
		this->positions[0] = glm::vec3(col, map.mapHeight[row][col], row);
		this->positions[1] = glm::vec3(col, map.mapHeight[row + 1][col], row + 1);
		this->positions[2] = glm::vec3(col + 1, map.mapHeight[row][col + 1], row);
		this->positions[3] = glm::vec3(col + 1, map.mapHeight[row + 1][col + 1], row + 1);
	}

};
