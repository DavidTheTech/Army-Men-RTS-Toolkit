#include "PerlinNoise.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// Default permutation (classic) – used only if never seeded
static const int defaultPerm[] = {
    151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
    190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,
    171,168,68,175,74,165,71,134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,
    245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208,89,18,169,200,196,135,130,116,188,159,86,
    164,100,109,198,173,186,3,64,52,217,226,250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,
    58,17,182,189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,172,9,129,22,39,253,19,
    98,108,110,79,113,224,232,178,185,112,104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,
    145,235,249,14,239,107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,138,236,205,
    93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

PerlinNoise::PerlinNoise()
{
    // Start with default permutation
    for (int i = 0; i < 256; i++)
    {
        perm[i] = defaultPerm[i];
        perm[i + 256] = defaultPerm[i];
    }
    // Seed with time to get different results per run (optional)
    SetSeed((unsigned int)std::time(nullptr));
}

void PerlinNoise::SetSeed(unsigned int seed)
{
    // Create a new permutation array based on the seed
    int p[256];
    for (int i = 0; i < 256; i++)
        p[i] = i;

    // Fisher-Yates shuffle using the seed
    std::srand(seed);
    for (int i = 255; i > 0; i--)
    {
        int j = std::rand() % (i + 1);
        std::swap(p[i], p[j]);
    }

    // Duplicate to avoid overflow
    for (int i = 0; i < 256; i++)
    {
        perm[i] = p[i];
        perm[i + 256] = p[i];
    }
}

float PerlinNoise::grad(int hash, float x, float y)
{
    int h = hash & 7;
    float u = (h < 4) ? x : y;
    float v = (h < 4) ? y : x;
    float uVal = ((h & 1) == 0) ? u : -u;
    float vVal = ((h & 2) == 0) ? v : -v;
    return uVal + vVal;
}

float PerlinNoise::RawNoise(float x, float y)
{
    int X = (int)std::floor(x) & 255;
    int Y = (int)std::floor(y) & 255;

    x -= std::floor(x);
    y -= std::floor(y);

    float u = fade(x);
    float v = fade(y);

    int aa = perm[perm[X] + Y];
    int ab = perm[perm[X] + Y + 1];
    int ba = perm[perm[X + 1] + Y];
    int bb = perm[perm[X + 1] + Y + 1];

    float res = lerp(
        lerp(grad(aa, x, y), grad(ba, x - 1, y), u),
        lerp(grad(ab, x, y - 1), grad(bb, x - 1, y - 1), u),
        v
    );

    return (res + 1.0f) / 2.0f;
}

float PerlinNoise::Noise(float x, float y)
{
    return RawNoise(x, y);
}

float PerlinNoise::NoiseScaled(float x, float y, float scale)
{
    // scale > 1  -> more dense (higher frequency)
    // scale < 1  -> more stretched
    return RawNoise(x * scale, y * scale);
}

float PerlinNoise::NoiseOctave(float x, float y, int octaves, float persistence, float lacunarity)
{
    float amplitude = 1.0f;
    float frequency = 1.0f;
    float noiseValue = 0.0f;
    float maxAmplitude = 0.0f;

    for (int i = 0; i < octaves; i++)
    {
        noiseValue += RawNoise(x * frequency, y * frequency) * amplitude;
        maxAmplitude += amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }

    return noiseValue / maxAmplitude;
}

float PerlinNoise::NoiseRange(float x, float y, float minVal, float maxVal)
{
    float t = RawNoise(x, y);
    return minVal + t * (maxVal - minVal);
}