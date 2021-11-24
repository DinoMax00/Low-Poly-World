#pragma once


#include <cmath>
#include "base.h"

//颜色生成类
class ColourGenerator
{
private:
	glm::vec3 biomeColours[MAX_SIZE];
	float part;

public:
	/**
	 * @param biomeColours
	 *            - The preset colours that will be interpolated over the
	 *            terrain. The first colours in this array will be used for the
	 *            lowest parts of the terrain, and the last colours in this
	 *            array will be used for the highest. All the other colours will
	 *            be spread out linearly inbetween.
	 * @param spread
	 *            - This indicates how much of the possible altitude range the
	 *            colours should be spread over. If this is too high the extreme
	 *            colours won't be used as there won't be any terrain vertices
	 *            high or low enough (the heights generator doesn't usually fill
	 *            the whole altitude range).
	 */
	//构造函数，数组第一位为最低位颜色，最后一位为最高位颜色，中间高度颜色渐变
	//传参为颜色数组、颜色数组长度、传播速度
	ColourGenerator(){}
	ColourGenerator(glm::vec3 biomeColours[],int lenth)
	{
		for(int i=0;i<lenth;i++)
			this->biomeColours[i] = biomeColours[i];
		this->part = 1.0 / (lenth - 1.0);
	}
	/*void show()
	{
		for(int i=1;i<lenth;i++)
	}*/
	/**
	 * Calculates the colour for every vertex of the terrain, by linearly
	 * interpolating between the biome colours depending on the vertex's height.
	 *
	 * @param heights
	 *            -The heights of all the vertices in the terrain.
	 * @param amplitude
	 *            - The amplitude range of the terrain that was used in the
	 *            heights generation. Maximum possible height is
	 *            {@code altitude} and minimum possible is {@code -altitude}.
	 * @return The colours of all the vertices in the terrain, in a grid.
	 */
	//生成颜色，传参为幅度
	void generateColours() 
	{
		std::cout << "here generateColours" << std::endl;
		float maxAmplitude = -10000.0, minAmplitude=10000.0;
		for (int z = 0; z < map.mapSize; z++)
		{
			for (int x = 0; x < map.mapSize; x++)
			{
				maxAmplitude = maxAmplitude > map.mapHeight[z][x]? maxAmplitude: map.mapHeight[z][x];
				minAmplitude = minAmplitude < map.mapHeight[z][x] ? minAmplitude : map.mapHeight[z][x];
			}
				
		}
		for (int z = 0; z < map.mapSize; z++) 
		{
			for (int x = 0; x < map.mapSize; x++) 
			{
				map.mapColour[z][x] = calculateColour(map.mapHeight[z][x], maxAmplitude- minAmplitude, minAmplitude);
			}
		}
	}

	/**Determines the colour of the vertex based on the provided height.
	 * @param height - Height of the vertex.
	 * @param amplitude - The maximum height that a vertex can be (
	 * @return
	 */
	//计算颜色
	glm::vec3 calculateColour(float height, float amplitude,float minamplitude)
	{
		float value = (height - minamplitude) / amplitude ;
		//value =(value - halfSpread) * (1.0 / spread);
		//value = value < 0.0 ? 0.0 : (value > 0.9999f ? 0.9999f : value);
		int firstBiome = (int)floor(value / part);
		float blend = (value - (firstBiome * part)) / part;
		float colour1Weight = 1 - blend;
		float r = (colour1Weight * biomeColours[firstBiome].r) + (blend * biomeColours[firstBiome + 1].r);
		float g = (colour1Weight * biomeColours[firstBiome].g) + (blend * biomeColours[firstBiome + 1].g);
		float b = (colour1Weight * biomeColours[firstBiome].b) + (blend * biomeColours[firstBiome + 1].b);
		//std::cout << "fb=" << firstBiome<<std::endl;
		return glm::vec3{ r, g, b };
	}

};
