#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../rendering/WaterRender.hpp"

#include "../config.h"

/**
 * 水的生成.
 */

constexpr int VERTICES_PER_SQUARE = 3 * 2; /// 每个正方形顶点数
constexpr int VERTEX_SIZE_BYTES = 8 + 4;

class WaterGenerator {

private:
	glm::vec2* buffer = NULL;
	unsigned int buffer_idx = 0;

public:

	WaterRender* generate() {
		int vertex_count = MAP_SIZE * MAP_SIZE * VERTICES_PER_SQUARE;
		createMeshData(MAP_SIZE, vertex_count);
		return new WaterRender(vertex_count * 3, buffer);
	}

private:

	 void createMeshData(int gridCount, unsigned int totalVertexCount) {
		 buffer = new glm::vec2[totalVertexCount * 3ll];
		 for (int row = 0; row < gridCount; row++) {
			for (int col = 0; col < gridCount; col++) {
				storeGridSquare(col, row);
		 	}
		 }
	 }

	 void storeGridSquare(int col, int row) {
		 auto cornerPos = calculateCornerPositions(col, row);
		 storeTriangle(cornerPos, true);
		 storeTriangle(cornerPos, false);
	 }

	 void storeTriangle(glm::vec2* cornerPos, bool left) {
		 int index0 = left ? 0 : 2;
		 int index1 = 1;
		 int index2 = left ? 2 : 3;

		 getIndicators(index0, cornerPos, index1, index2);
		 getIndicators(index1, cornerPos, index2, index0);
		 getIndicators(index2, cornerPos, index0, index1);
	 }

	 glm::vec2* calculateCornerPositions(int col, int row) {
		 auto vertices = new glm::vec2[4];
		 vertices[0] = glm::vec2(col, row);
		 vertices[1] = glm::vec2(col, row + 1);
		 vertices[2] = glm::vec2(col + 1, row);
		 vertices[3] = glm::vec2(col + 1, row + 1);
		 return vertices;
	 }

	 void getIndicators(int currentVertex, glm::vec2* vertexPositions, int vertex1, int vertex2) {
		 glm::vec2 currentVertexPos = vertexPositions[currentVertex];
		 glm::vec2 vertex1Pos = vertexPositions[vertex1];
		 glm::vec2 vertex2Pos = vertexPositions[vertex2];
		 glm::vec2 offset1 = vertex1Pos - currentVertexPos;
		 glm::vec2 offset2 = vertex2Pos - currentVertexPos;

		 buffer[buffer_idx++] = vertexPositions[currentVertex];
		 buffer[buffer_idx++] = offset1;
		 buffer[buffer_idx++] = offset2;
	 }
};





