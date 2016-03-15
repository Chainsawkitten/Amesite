#include "PerlinNoise.hpp"
#include <random>
#include <numeric>
#include <algorithm>
#include <glm\glm.hpp>

PerlinNoiseGenerator::PerlinNoiseGenerator(unsigned int seed) {
    mPermutationVector.resize(256);

    std::iota(mPermutationVector.begin(), mPermutationVector.end(), 0);

    std::default_random_engine engine(seed);

    std::shuffle(mPermutationVector.begin(), mPermutationVector.end(), engine);

    mPermutationVector.insert(mPermutationVector.end(), mPermutationVector.begin(), mPermutationVector.end());;
}

float PerlinNoiseGenerator::Noise(float x, float y, float z) {
    int X = static_cast<int>(glm::floor(x)) & 255;
    int Y = static_cast<int>(glm::floor(y)) & 255;
    int Z = static_cast<int>(glm::floor(z)) & 255;

    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    float u = Fade(x);
    float v = Fade(y);
    float w = Fade(z);

    int A = mPermutationVector[X] + Y;
    int AA = mPermutationVector[A] + Z;
    int AB = mPermutationVector[A + 1] + Z;
    int B = mPermutationVector[X + 1] + Y;
    int BA = mPermutationVector[B] + Z;
    int BB = mPermutationVector[B + 1] + Z;
    float res = Lerp(w, Lerp(v, Lerp(u, Grad(mPermutationVector[AA], x, y, z), Grad(mPermutationVector[BA], x - 1, y, z)), Lerp(u, Grad(mPermutationVector[AB], x, y - 1, z), Grad(mPermutationVector[BB], x - 1, y - 1, z))), Lerp(v, Lerp(u, Grad(mPermutationVector[AA + 1], x, y, z - 1), Grad(mPermutationVector[BA + 1], x - 1, y, z - 1)), Lerp(u, Grad(mPermutationVector[AB + 1], x, y - 1, z - 1), Grad(mPermutationVector[BB + 1], x - 1, y - 1, z - 1))));
    return (res + 1.f / 2.f);

}

float PerlinNoiseGenerator::Fade(float t) {
    return t*t*t*(t*(t * 6.f - 15.f) + 10.f);
}

float PerlinNoiseGenerator::Lerp(float t, float a, float b) {
    return a + t * (b - a);
}

float PerlinNoiseGenerator::Grad(int hash, float x, float y, float z) {
    int h = hash & 15;
    double u = h < 8 ? x : y,
        v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}