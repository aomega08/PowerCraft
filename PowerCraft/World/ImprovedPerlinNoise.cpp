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

const double GRAD_X[] = {1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, 0.0};
const double GRAD_Y[] = {1.0, 1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0};
const double GRAD_Z[] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 0.0, 1.0, 0.0, -1.0};

double ImprovedPerlinNoise::grad(int t, double x, double y, double z) {
    t &= 15;
    return GRAD_X[t] * x + GRAD_Y[t] * y + GRAD_Z[t] * z;
}


void ImprovedPerlinNoise::populateNoiseArray(double *noiseArray,
                                             double xOffset, double yOffset, double zOffset,
                                             int xSize, int ySize, int zSize,
                                             double xScale, double yScale, double zScale,
                                             double noiseScale) {
    if (ySize == 1) {
        fill2D(noiseArray, xOffset, zOffset, xSize, zSize, xScale, zScale, noiseScale);
    } else {
        fill3D(noiseArray, xOffset, yOffset, zOffset, xSize, ySize, zSize, xScale, yScale, zScale, noiseScale);
    }
}

void ImprovedPerlinNoise::fill3D(double *noiseArray,
                                 double xOffset, double yOffset, double zOffset,
                                 int xSize, int ySize, int zSize,
                                 double xScale, double yScale, double zScale,
                                 double noiseScale) {

    double inverseNoiseScale = 1.0 / noiseScale;

    for (int X = 0; X < xSize; X++) {
        double xFactor = xOffset + xCoord + X * xScale;

        // Round toward 0
        int xInt = (int) xFactor;
        if (xFactor < (double) xInt)
            xInt--;

        // Mod 256
        xInt &= 255;

        // Take decimal part
        xFactor -= (double) xInt;

        // Compute LERP parameters
        double u = perlinFade(xFactor);

        for (int Z = 0; Z < zSize; Z++) {
            double zFactor = zOffset + zCoord + Z * zScale;

            int zInt = (int) zFactor;
            if (zFactor < (double) zInt)
                zInt--;

            zInt &= 255;
            zFactor -= (double) zInt;
            double w = perlinFade(zFactor);

            for (int Y = 0; Y < ySize; Y++) {
                double yFactor = yOffset + yCoord + Y * yScale;

                int yInt = (int) yFactor;
                if (yFactor < (double) yInt)
                    yInt--;

                yInt &= 255;
                yFactor -= (double) yInt;
                double v = perlinFade(yFactor);

                *noiseArray++ += perlin(xInt, xFactor, u,
                                        yInt, yFactor, v,
                                        zInt, zFactor, w) * inverseNoiseScale;
            }
        }
    }
}

void ImprovedPerlinNoise::fill2D(double *noiseArray,
                                 double xOffset, double zOffset,
                                 int xSize, int zSize,
                                 double xScale, double zScale,
                                 double noiseScale) {

    double inverseNoiseScale = 1.0 / noiseScale;

    for (int X = 0; X < xSize; X++) {
        double xFactor = xOffset + xCoord + X * xScale;

        // Round toward 0
        int xInt = (int) xFactor;
        if (xFactor < (double) xInt)
            xInt--;

        // Mod 256
        xInt &= 255;

        // Take decimal part
        xFactor -= (double) xInt;

        // Compute LERP parameters
        double u = perlinFade(xFactor);

        for (int Z = 0; Z < zSize; Z++) {
            double zFactor = zOffset + zCoord + Z * zScale;

            int zInt = (int) zFactor;
            if (zFactor < (double) zInt)
                zInt--;

            zInt &= 255;
            zFactor -= (double) zInt;
            double w = perlinFade(zFactor);

            *noiseArray++ += perlin(xInt, xFactor, u,
                                    0, 0.0, 0,
                                    zInt, zFactor, w) * inverseNoiseScale;
        }
    }
}

double ImprovedPerlinNoise::perlin(int X, double xFactor, double u,
                                   int Y, double yFactor, double v,
                                   int Z, double zFactor, double w) {
    int A = permutations[X] + Y;
    int AA = permutations[A] + Z;
    int AB = permutations[A + 1] + Z;

    int B = permutations[X + 1] + Y;
    int BA = permutations[B] + Z;
    int BB = permutations[B + 1] + Z;

    double lerpX1 = lerp(u, grad(permutations[AA], xFactor, yFactor, zFactor),
                  grad(permutations[BA], xFactor - 1.0, yFactor, zFactor));
    double lerpX2 = lerp(u, grad(permutations[AB], xFactor, yFactor - 1.0, zFactor),
                  grad(permutations[BB], xFactor - 1.0, yFactor - 1.0, zFactor));
    double lerpX3 = lerp(u, grad(permutations[AA + 1], xFactor, yFactor, zFactor - 1.0),
                  grad(permutations[BA + 1], xFactor - 1.0, yFactor, zFactor - 1.0));
    double lerpX4 = lerp(u, grad(permutations[AB + 1], xFactor, yFactor - 1.0, zFactor - 1.0),
                  grad(permutations[BB + 1], xFactor - 1.0, yFactor - 1.0, zFactor - 1.0));

    double lerpY1 = lerp(v, lerpX1, lerpX2);
    double lerpY2 = lerp(v, lerpX3, lerpX4);

    return lerp(w, lerpY1, lerpY2);
}
