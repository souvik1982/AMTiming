#pragma once

#include <array>

struct EventCharacteristics
{
  std::array<double, 6> nStubs_layer;
  double nPatterns;
  double nOutwords;
  double nCombinations;
  double nTracks;
};
