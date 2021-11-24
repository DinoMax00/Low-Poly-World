#pragma once
//地图最大边长
#define MAX_SIZE 1005
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
//储存最终地图地形所有信息
struct MAP
{
	//地图颜色信息，分别保存每个顶点颜色
	glm::vec3 mapColour[MAX_SIZE][MAX_SIZE];
	//地图高度信息，分别保存每个顶点高度
	float mapHeight[MAX_SIZE][MAX_SIZE];
	//地图顶点坐标信息，分别保存每个顶点坐标，由于要拆点所以内存额外分配
	glm::vec3 terrainData[MAX_SIZE * MAX_SIZE * 3];
	//地图顶点数组下标
	int dataIndex = 0;
	//地图三角新信息，保存每个三角形三个顶点信息
	int terrainIndices[MAX_SIZE * MAX_SIZE * 3];
	int indicesIndex = 0;
	//地图边长
	int mapSize;
}map;