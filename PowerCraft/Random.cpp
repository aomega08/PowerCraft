#include "stdafx.h"
#include "Random.h"

#include <cstdint>
#include <ctime>

Random::Random() : Random(time(NULL)) {
}

Random::Random(uint64_t seed) {
  SetSeed(seed);
}

void Random::SetSeed(uint64_t seed) {
  _seed = (seed ^ 0x5DEECE66DL) & ((1LL << 48) - 1);
}

uint32_t Random::Next(int bits) {
  _seed = (_seed * 0x5DEECE66DL + 0xBL) & ((1LL << 48) - 1);
  return (uint32_t)(_seed >> (48 - bits));
}

void Random::NextBytes(uint8_t *bytes, size_t n) {
  for (int i = 0; i < n; i++) {
    for (int rnd = NextInt(), t = (n - i < 4 ? n - i : 4); t-- > 0; rnd >>= 8)
      bytes[i] = (uint8_t)rnd;
  }
}

uint32_t Random::NextInt() {
  return Next(32);
}

uint32_t Random::NextInt(int n) {
  if (n <= 0)
    throw "n must be positive";

  if ((n & -n) == n)
    return (int)((n * (uint64_t)Next(31)) >> 31);

  int bits, val;
  do {
    bits = Next(31);
    val = bits % n;
  } while (bits - val + (n - 1) < 0);

  return val;
}

uint64_t Random::NextLong() {
  return ((uint64_t)Next(32) << 32) + Next(32);
}

bool Random::NextBoolean() {
  return Next(1) != 0;
}

float Random::NextFloat() {
  return Next(24) / ((float)(1L << 24));
}

double Random::NextDouble() {
  return (((uint64_t)Next(26) << 27) + Next(27)) / (double)(1LL << 53);
}

