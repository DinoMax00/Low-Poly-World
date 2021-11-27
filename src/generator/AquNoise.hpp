#pragma once

#include <stdlib.h>
#include <iostream>
#include <ctime>

/**
 * °¢ÇüµÄÅ£±ÆËã·¨.
 */
class AquNoise {
private:
	int p[512], permutation[256];
public:

	AquNoise()
	{
		srand(time(0));
		for (int i = 0; i < 256; i++)
			permutation[i] = rand() % 256;
	}

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

	float noiseLayer(float x, float y, float lacunarity = 2, float persistance = .5, int octaves = 16)
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
};
