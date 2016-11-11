#pragma once

#include <stddef.h>
#include <stdint.h>

class Random {
public:
  Random();
  Random(uint64_t seed);

  void SetSeed(uint64_t seed);

  void NextBytes(uint8_t *bytes, size_t n);
  uint32_t NextInt();
  uint32_t NextInt(int n);
  uint64_t NextLong();
  bool NextBoolean();
  float NextFloat();
  double NextDouble();

protected:
  uint32_t Next(int bits);

private:
  uint64_t _seed;
};

