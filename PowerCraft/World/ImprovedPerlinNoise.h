#pragma once

#include "../Random.h"

class ImprovedPerlinNoise {
public:
    ImprovedPerlinNoise();
    ImprovedPerlinNoise(Random random);

    double lerp(double t, double a, double b);
    double grad(int t, double x, double y, double z);

    void populateNoiseArray(double *noiseArray, double xOffset, double yOffset, double zOffset, int xSize, int ySize, int zSize, double xScale, double yScale, double zScale, double noiseScale);

private:
    int permutations[512];

    double xCoord, yCoord, zCoord;

    void fill2D(double *noiseArray, double xOffset, double zOffset, int xSize, int zSize, double xScale, double zScale,
                double noiseScale);

    void fill3D(double *noiseArray, double xOffset, double yOffset, double zOffset, int xSize, int ySize, int zSize,
               double xScale, double yScale, double zScale, double noiseScale);

    double perlinFade(double v) const;

    double perlin(double xFactor, double zFactor, double yFactor);
};
