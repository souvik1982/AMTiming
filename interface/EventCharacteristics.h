#pragma once

#include <array>
#include <vector>

class EventCharacteristics
{
  public: 
    EventCharacteristics(std::vector<float> *stubs_modId, std::vector<float> *stubs_r, std::vector<std::vector<std::vector<unsigned int> > > *roads_stubRefs, std::vector<float> *tracks_eta);
  
  public: 
    std::array<double, 6> nStubs_layer;
    double nPatterns;
    double nOutwords;
    double nCombinations;
    double nTracks;
};
