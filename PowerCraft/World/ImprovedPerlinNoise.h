#pragma once

#include "../Random.h"

class ImprovedPerlinNoise {
public:
    ImprovedPerlinNoise();
    ImprovedPerlinNoise(Random random);

    inline double lerp(double t, double a, double b) {
        return a + t * (b - a);
    }

    double grad(int t, double x, double y, double z);

    void populateNoiseArray(double *noiseArray, double xOffset, double yOffset, double zOffset, int xSize, int ySize, int zSize, double xScale, double yScale, double zScale, double noiseScale);

private:
    int permutations[512];

    double xCoord, yCoord, zCoord;

    void fill2D(double *noiseArray, double xOffset, double zOffset, int xSize, int zSize, double xScale, double zScale,
                double noiseScale);

    void fill3D(double *noiseArray, double xOffset, double yOffset, double zOffset, int xSize, int ySize, int zSize,
               double xScale, double yScale, double zScale, double noiseScale);

    inline double perlinFade(double v) const {
        // 6v^5 - 15v^4 + 10v^3
        return v * v * v * (v * (v * 6.0 - 15.0) + 10.0);
    }

    double perlin(int X, double xFactor, double u,
                  int Y, double yFactor, double v,
                  int Z, double zFactor, double w);
};
