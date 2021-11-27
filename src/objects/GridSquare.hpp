#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "../config.h"

typedef glm::vec3 color;

//方格类，将每个方格分割为两个三角形
class GridSquare {

//private:
public:
	int row = 0;
	int col = 0;
	int lastIndex = 0;
	glm::vec3* positions = NULL;
	color* colors = NULL;
	// 两三角形法向量
	glm::vec3 normalLeft;
	glm::vec3 normalRight;

public:
	//构造函数
	GridSquare() {}
	GridSquare(int row, int col, float** height_map, color** color_map)
	{
		calculateCornerPositions(col, row, height_map);
		calculateCornercolors(col, row, color_map);
		this->lastIndex = MAP_SIZE - 1;
		this->row = row;
		this->col = col;
		bool rightHanded = (col % 2 != row % 2);
		this->normalLeft = glm::normalize(glm::cross(positions[1] - positions[0], positions[rightHanded ? 3 : 2] - positions[0]));
		this->normalRight = glm::normalize(glm::cross(positions[rightHanded ? 0 : 1] - positions[2], positions[3] - positions[2]));
	}

	/**
	 * 存储方格信息，包括关键点坐标，颜色，法向量.
	 * 
	 * \param buffers 存储位置
	 * \param idx 当前下标
	 */
	void storeSquareData(glm::vec3* buffers, int& idx)
	{
		buffers[idx++] = positions[0];
		buffers[idx++] = normalLeft;
		buffers[idx++] = colors[0];
		if (row != lastIndex || col == lastIndex)
		{
			buffers[idx++] = positions[2];
			buffers[idx++] = normalRight;
			buffers[idx++] = colors[2];
		}
	}

	/**
	 * 记录最后一行方格信息到顶点数组中.
	 * 
	 * \param buffers
	 * \param idx
	 */
	void storeBottomRowData(glm::vec3* buffers, int& idx)
	{
		if (col == 0)
		{
			buffers[idx++] = positions[1];
			buffers[idx++] = normalLeft;
			buffers[idx++] = colors[1];
		}
		buffers[idx++] = positions[3];
		buffers[idx++] = normalRight;
		buffers[idx++] = colors[3];
	}

	/**
	 * 计算四个顶点的颜色.
	 * 
	 * \param col
	 * \param row
	 * \param color_map
	 */
	void calculateCornercolors(int col, int row, color** color_map)
	{
		this->colors = new color[4];
		this->colors[0] = color_map[row][col];
		this->colors[1] = color_map[row + 1][col];
		this->colors[2] = color_map[row][col + 1];
		this->colors[3] = color_map[row + 1][col + 1];
	}

	/**
	 * 计算正方形顶点坐标.
	 *  左上、左下、右上、右下
	 * \param col 
	 * \param row 
	 * \param height_map
	 */
	void calculateCornerPositions(int col, int row, float** height_map)
	{
		this->positions = new glm::vec3[4];
		this->positions[0] = glm::vec3(col, height_map[row][col], row);
		this->positions[1] = glm::vec3(col, height_map[row + 1][col], row + 1);
		this->positions[2] = glm::vec3(col + 1, height_map[row][col + 1], row);
		this->positions[3] = glm::vec3(col + 1, height_map[row + 1][col + 1], row + 1);
	}

};
