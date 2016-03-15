#pragma once
#include <vector>

///Perlin Noise Generator
class PerlinNoiseGenerator {
    public:
        ///Constructor
        /**
         @param seed Seed to use for the generator.
         */
        PerlinNoiseGenerator(unsigned int seed);

        ///Gets noise for given position.
        /**
         @param x X-position.
         @param y Y-position.
         @param z Z-position.
         @return Value for given position.
         */
        float Noise(float x, float y, float z);
    private:
        float Fade(float t);

        float Lerp(float t, float a, float b);

        float Grad(int hash, float x, float y, float z);
        std::vector<int> mPermutationVector;
};