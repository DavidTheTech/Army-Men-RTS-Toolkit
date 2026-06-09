// PerlinNoise.h
#pragma once

class PerlinNoise
{
private:
    int perm[512];

    float lerp(float a, float b, float t)
    {
        return a + (b - a) * t;
    }

    float fade(float t)
    {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    float grad(int hash, float x, float y);
    float RawNoise(float x, float y);

public:
    PerlinNoise();
    void SetSeed(unsigned int seed);

    float Noise(float x, float y);

    float NoiseScaled(float x, float y, float scale);
    float NoiseOctave(float x, float y, int octaves, float persistence, float lacunarity);
    float NoiseRange(float x, float y, float minVal, float maxVal);
};