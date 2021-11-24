#pragma once
//��ͼ���߳�
#define MAX_SIZE 1005
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
//�������յ�ͼ����������Ϣ
struct MAP
{
	//��ͼ��ɫ��Ϣ���ֱ𱣴�ÿ��������ɫ
	glm::vec3 mapColour[MAX_SIZE][MAX_SIZE];
	//��ͼ�߶���Ϣ���ֱ𱣴�ÿ������߶�
	float mapHeight[MAX_SIZE][MAX_SIZE];
	//��ͼ����������Ϣ���ֱ𱣴�ÿ���������꣬����Ҫ��������ڴ�������
	glm::vec3 terrainData[MAX_SIZE * MAX_SIZE * 3];
	//��ͼ���������±�
	int dataIndex = 0;
	//��ͼ��������Ϣ������ÿ������������������Ϣ
	int terrainIndices[MAX_SIZE * MAX_SIZE * 3];
	int indicesIndex = 0;
	//��ͼ�߳�
	int mapSize;
}map;