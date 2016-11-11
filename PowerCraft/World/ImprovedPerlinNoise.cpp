#include <stdint.h>
#include <stdio.h>

#include "ImprovedPerlinNoise.h"

ImprovedPerlinNoise::ImprovedPerlinNoise() : ImprovedPerlinNoise(Random()) {
}

ImprovedPerlinNoise::ImprovedPerlinNoise(Random random) {
    xCoord = random.NextDouble() * 256;
    yCoord = random.NextDouble() * 256;
    zCoord = random.NextDouble() * 256;

    for (int i = 0; i < 256; i++) {
        permutations[i] = i;
    }

    for (int i = 0; i < 256; i++) {
        int j = random.NextInt(256 - i) + i;
        int k = permutations[i];
        permutations[i] = permutations[i + 256] = permutations[j];
        permutations[j] = k;
    }
}

inline double ImprovedPerlinNoise::lerp(double t, double a, double b) {
    return a + t * (b - a);
}

inline double ImprovedPerlinNoise::grad(int t, double x, double y, double z) {
    t &= 15;
    double u = t < 8 ? x : y;
    double v = t < 4 ? y : (t == 12 || t == 14) ? x : z;

    return ((t & 1) == 0 ? u : -u) + ((t & 2) == 0 ? v : -v);
}


inline double ImprovedPerlinNoise::perlinFade(double v) const {
    // 6v^5 - 15v^4 + 10v^3
    return v * v * v * (v * (v * 6.0 - 15.0) + 10.0);
}

void ImprovedPerlinNoise::populateNoiseArray(double *noiseArray,
                                                double xOffset, double yOffset, double zOffset,
                                                int xSize, int ySize, int zSize,
                                                double xScale, double yScale, double zScale,
                                                double noiseScale) {
    fill3D(noiseArray, xOffset, yOffset, zOffset, xSize, ySize, zSize, xScale, yScale, zScale, noiseScale);
}

void ImprovedPerlinNoise::fill3D(double *noiseArray,
                                    double xOffset, double yOffset, double zOffset,
                                    int xSize, int ySize, int zSize,
                                    double xScale, double yScale, double zScale,
                                    double noiseScale) {

    double inverseNoiseScale = 1.0 / noiseScale;

    for (int X = 0; X < xSize; X++) {
        for (int Z = 0; Z < zSize; Z++) {
            for (int Y = 0; Y < ySize; Y++) {
                double xFactor = xOffset + xCoord + X * xScale;
                double zFactor = zOffset + zCoord + Z * zScale;
                double yFactor = yOffset + yCoord + Y * yScale;

                *noiseArray++ = perlin(xFactor, yFactor, zFactor) * inverseNoiseScale;
            }
        }
    }
}

double ImprovedPerlinNoise::perlin(double xFactor, double yFactor, double zFactor) {
    // Round toward 0
    int X = (int) xFactor;
    if (xFactor < (double) X)
        X--;

    int Z = (int) zFactor;
    if (zFactor < (double) Z)
        Z--;

    int Y = (int) yFactor;
    if (yFactor < (double) Y)
        Y--;

    // Mod 256
    X &= 255;
    Y &= 255;
    Z &= 255;

    // Take decimal part
    xFactor -= (double) X;
    yFactor -= (double) Y;
    zFactor -= (double) Z;

    // Compute LERP parameters
    double u = perlinFade(xFactor);
    double v = perlinFade(yFactor);
    double w = perlinFade(zFactor);

    int A = permutations[X] + Y;
    int AA = permutations[A] + Z;
    int AB = permutations[A + 1] + Z;

    int B = permutations[X + 1] + Y;
    int BA = permutations[B] + Z;
    int BB = permutations[B + 1] + Z;

    double lerpX1 = lerp(u, grad(permutations[AA],     xFactor,       yFactor,       zFactor),
                            grad(permutations[BA],     xFactor - 1.0, yFactor,       zFactor));
    double lerpX2 = lerp(u, grad(permutations[AB],     xFactor,       yFactor - 1.0, zFactor),
                            grad(permutations[BB],     xFactor - 1.0, yFactor - 1.0, zFactor));
    double lerpX3 = lerp(u, grad(permutations[AA + 1], xFactor,       yFactor,       zFactor - 1.0),
                            grad(permutations[BA + 1], xFactor - 1.0, yFactor,       zFactor - 1.0));
    double lerpX4 = lerp(u, grad(permutations[AB + 1], xFactor,       yFactor - 1.0, zFactor - 1.0),
                            grad(permutations[BB + 1], xFactor - 1.0, yFactor - 1.0, zFactor - 1.0));

    double lerpY1 = lerp(v, lerpX1, lerpX2);
    double lerpY2 = lerp(v, lerpX3, lerpX4);

    return lerp(w, lerpY1, lerpY2);
}

#if 0
#include <sys/time.h>

int main() {
    double array[16 * 16 * 16];

    struct timeval timeStart, timeEnd;
    gettimeofday(&timeStart, NULL);

    for (int i = 0; i < 100; i++) {
        Random rand((uint64_t) 0);
        ImprovedPerlinNoise noise(rand);
        noise.populateNoiseArray(array, 1, 2, 3, 16, 16, 16, 1.0, 2.0, 0.5, 2.0);
    }

    gettimeofday(&timeEnd, NULL);

    long usec = (timeEnd.tv_sec - timeStart.tv_sec) * 1000000 + timeEnd.tv_usec - timeStart.tv_usec;

    printf("%ld\n", usec);
}
#endif