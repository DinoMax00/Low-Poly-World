#pragma once

#include "base.h"

//�����±�������
class SpecialIndexGenerator
{

public:
	//���ɵ���
	static int generateIndexBuffer(int vertexCount)
	{
		int rowLength = (vertexCount - 1) * 2;
		int pointer = storeTopSection(rowLength, vertexCount);
		pointer = storeSecondLastLine(pointer, rowLength, vertexCount);
		pointer = storeLastLine( pointer, rowLength, vertexCount);
		return pointer;
	}

	//����ǰ���ձ黮�ֹ�����������Ϣ
	static int storeTopSection(int rowLength, int vertexLength)
	{
		int pointer = 0;
		for (int row = 0; row < vertexLength - 3; row++)
		{
			for (int col = 0; col < vertexLength - 1; col++)
			{
				int topLeft = (row * rowLength) + (col * 2);
				int topRight = topLeft + 1;
				int bottomLeft = topLeft + rowLength;
				int bottomRight = bottomLeft + 1;
				pointer = storeQuad(topLeft, topRight, bottomLeft, bottomRight, pointer, col % 2 != row % 2);
			}
		}
		return pointer;
	}

	//���浹���ڶ�����������Ϣ
	static int storeSecondLastLine( int pointer, int rowLength, int vertexLength)
	{
		int row = vertexLength - 3;
		for (int col = 0; col < vertexLength - 1; col++)
		{
			int topLeft = (row * rowLength) + (col * 2);
			int topRight = topLeft + 1;
			int bottomLeft = (topLeft + rowLength) - col;
			int bottomRight = bottomLeft + 1;
			pointer = storeQuad(topLeft, topRight, bottomLeft, bottomRight, pointer, col % 2 != row % 2);
		}
		return pointer;
	}

	//�������һ����������Ϣ
	static int storeLastLine( int pointer, int rowLength, int vertexLength)
	{
		int row = vertexLength - 2;
		for (int col = 0; col < vertexLength - 1; col++)
		{
			int topLeft = (row * rowLength) + col;
			int topRight = topLeft + 1;
			int bottomLeft = (topLeft + vertexLength);
			int bottomRight = bottomLeft + 1;
			pointer = storeLastRowQuad(topLeft, topRight, bottomLeft, bottomRight, pointer, col % 2 != row % 2);
		}
		return pointer;
	}

	static int storeQuad(int topLeft, int topRight, int bottomLeft, int bottomRight,int pointer,bool rightHanded)
	{
		pointer = storeLeftTriangle(topLeft, topRight, bottomLeft, bottomRight, pointer, rightHanded);
		map.terrainIndices[pointer++] = topRight;
		map.terrainIndices[pointer++] = rightHanded ? topLeft : bottomLeft;
		map.terrainIndices[pointer++] = bottomRight;
		return pointer;
	}

	static int storeLastRowQuad(int topLeft, int topRight, int bottomLeft, int bottomRight, int pointer,bool rightHanded)
	{
		pointer = storeLeftTriangle(topLeft, topRight, bottomLeft, bottomRight, pointer, rightHanded);
		map.terrainIndices[pointer++] = bottomRight;
		map.terrainIndices[pointer++] = topRight;
		map.terrainIndices[pointer++] = rightHanded ? topLeft : bottomLeft;
		return pointer;
	}

	static int storeLeftTriangle(int topLeft, int topRight, int bottomLeft, int bottomRight, int pointer,bool rightHanded)
	{
		map.terrainIndices[pointer++] = topLeft;
		map.terrainIndices[pointer++] = bottomLeft;
		map.terrainIndices[pointer++] = rightHanded ? bottomRight : topRight;
		return pointer;
	}

};
