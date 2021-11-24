#pragma once

#include "ColourGenerater.h"
#include "base.h"
#include "GridSquare.h"
#include "IndexGenerator.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "lib/camera.h"
#include "lib/shader.h"

//地图顶点生成类
class TerrainGenerator
{
private:
	//颜色类
	ColourGenerator colourGen;
	unsigned int VAO, VBO, EBO;
public:
	//构造函数
	TerrainGenerator( ColourGenerator colourGen) 
	{
		this->colourGen = colourGen;
	}

	/**
	 * Generates a terrain. First the heights and colours of all the vertices
	 * are generated.
	 *
	 * @param gridSize
	 *            - The number of grid squares along one side of the terrain.
	 * @return The generated terrain.
	 */
	void generateTerrain(int gridSize) 
	{
		std::cout << "here generateTerrain" << std::endl;
		for (int i = 0; i < 256; i++)
			permutation[i] = rand() % 256;
		map.mapSize = gridSize;
		generateHeights(gridSize);
		colourGen.generateColours();
		/*for (int z = 0; z < map.mapSize; z++)
		{
			for (int x = 0; x < map.mapSize; x++)
			{
				std::cout<<map.mapColour[z][x].x<<" "<< map.mapColour[z][x].y << " "<<map.mapColour[z][x].z << "\n";
			}
		}*/
		createTerrain();
	}

	/**
	 * For use when the app closes.
	 */

	/**
	 * Generates the terrain mesh data, loads it up to a VAO, and initializes
	 * the terrain.
	 *
	 * @param heights
	 *            - The heights of all the vertices in the terrain.
	 * @param colours
	 *            - The colours of all the vertices.
	 * @return The new terrain.
	 */

	/**
	 * Uses the perlin noise generator (which might actually not be using the
	 * Perlin Noise algorithm - I'm not quite sure if it is or isn't) to
	 * generate heights for all of the terrain's vertices.
	 *
	 * @param gridSize - The number of grid squares along one edge of the terrain.
	 * @param perlinNoise - The heights generator.
	 * @return All the heights for the vertices.
	 */
	//生成地图高度信息
	int p[512], permutation[256] /*= { 151,160,137,91,90,15,
		131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
		190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
		88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
		77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
		102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
		135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
		5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
		223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
		129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
		251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
		49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
		138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
	}*/;

	double fade(double t)
	{
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	double lerp(double t, double a, double b)
	{
		return a + t * (b - a);
	}

	double gradient(int hash, double x, double y, double z)
	{
		int h = hash & 15;
		double u = h < 8 ? x : y;
		double v = h < 4 ? y : h == 12 || h == 14 ? x : z;

		return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
	}

	double noise(double x, double y, double z)
	{
		for (int i = 0; i < 256; i++) p[256 + i] = p[i] = permutation[i];
		int x1 = (int)floor(x) & 255,
			y1 = (int)floor(y) & 255,
			z1 = (int)floor(z) & 255;

		x -= floor(x);
		y -= floor(y);
		z -= floor(z);

		double x2 = fade(x),
			y2 = fade(y),
			z2 = fade(z);

		int A = p[x1] + y1, AA = p[A] + z1, AB = p[A + 1] + z1,      // HASH COORDINATES OF
			B = p[x1 + 1] + y1, BA = p[B] + z1, BB = p[B + 1] + z1;      // THE 8 CUBE CORNERS,

		return lerp(z2, lerp(y2, lerp(x2, gradient(p[AA], x, y, z),
			gradient(p[BA], x - 1, y, z)),
			lerp(x2, gradient(p[AB], x, y - 1, z),
				gradient(p[BB], x - 1, y - 1, z))),
			lerp(y2, lerp(x2, gradient(p[AA + 1], x, y, z - 1),
				gradient(p[BA + 1], x - 1, y, z - 1)),
				lerp(x2, gradient(p[AB + 1], x, y - 1, z - 1),
					gradient(p[BB + 1], x - 1, y - 1, z - 1))));
	}

	float noiseLayer(float x, float y, float lacunarity, float persistance, int octaves) 
	{

		float freq = .5,
			ampl = 50;

		float n = 1;

		for (int i = 0; i < octaves; i++) {
			n += noise(x * freq, y * freq, 1230) * ampl;
			freq *= lacunarity;
			ampl *= persistance;
		}

		return n;
	}
#define  resolution  100
	void generateHeights(int gridSize) 
	{
		for (int z = 0; z < map.mapSize; z++) 
		{
			for (int x = 0; x < map.mapSize; x++) 
			{
				//map.mapHeight[z][x] =perlinNoise.getPerlinNoise(x, z);
				//
				map.mapHeight[z][x] = noiseLayer((float)z / resolution, (float)x / resolution, 2, .5, 16);
				//std::cout << map.mapHeight[z][x] << " ";
			}
		}

	}

	//生成地图三角形信息
	void createTerrain() 
	{
		int vertexCount = calculateVertexCount(map.mapSize);
		createMeshData(vertexCount);
		/*std::cout << "mapindex=" << map.dataIndex;*/
		map.indicesIndex=SpecialIndexGenerator::generateIndexBuffer(map.mapSize);
	}

	//计算总顶点数
	int calculateVertexCount(int vertexLength) 
	{
		int bottom2Rows = 2 * vertexLength;
		int remainingRowCount = vertexLength - 2;
		int topCount = remainingRowCount * (vertexLength - 1) * 2;
		return topCount + bottom2Rows;
	}

	//储存所有顶点信息
	void createMeshData(int vertexCount) 
	{
		GridSquare lastRow[MAX_SIZE];
		for (int row = 0; row < map.mapSize - 1; row++) 
		{
			for (int col = 0; col < map.mapSize - 1; col++) 
			{
				GridSquare square(row, col);
				square.storeSquareData();
				if (row == map.mapSize - 2) 
				{
					lastRow[col] = square;
				}
			}
		}
		for (int i = 0; i < map.mapSize-1; i++) 
		{
			lastRow[i].storeBottomRowData();
		}
	}

	//在循环外进行地图绑定
	void TerrainRender()
	{

		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		//// bind the Vertex Array Object first, then bind and set vertex buffer(s),
		/// and then configure vertex attributes(s).
		glBindVertexArray(this->VAO);

		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, map.dataIndex * sizeof(glm::vec3),
			map.terrainData, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, map.indicesIndex * sizeof(int),
			map.terrainIndices, GL_DYNAMIC_DRAW);
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void*)(1 * sizeof(glm::vec3)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void*)(2 * sizeof(glm::vec3)));
		glEnableVertexAttribArray(2);

		// 解除绑定
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	//进行地图绘制
	void TerrainDraw()
	{
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, map.indicesIndex, GL_UNSIGNED_INT, 0);
	}

	//进行shader调用，传入shader、camera以及光照位置与颜色
	void TerrainUseshader(Shader& ourShader, Camera& camera,glm::vec3 lightPos,glm::vec3 lightColour)
	{
		ourShader.use();
		ourShader.setVec3("lightColor", lightColour);
		ourShader.setVec3("lightPos", lightPos);
		ourShader.setVec3("viewPos", camera.Position);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		ourShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		ourShader.setMat4("model", model);

		ourShader.setFloat("opc", 1.0f);
	}

	//进行解绑与删除
	void TerrainDelete()
	{
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
		glDeleteBuffers(1, &this->EBO);
	}
};
