#pragma once
#include <vector>

class PerlinNoiseGenerator{
	public:
		PerlinNoiseGenerator(unsigned int seed);

		float Noise(float x, float y, float z);
	private:
		float Fade(float t);
		float Lerp(float t, float a, float b);
		float Grad(int hash, float x, float y, float z);
		std::vector<int> mPermutationVector;
};