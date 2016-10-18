#pragma once

#include <array>
#include <vector>

class EventCharacteristics
{
  public:
    EventCharacteristics();
    EventCharacteristics(std::vector<float> *stubs_modId, std::vector<float> *stubs_r, std::vector<std::vector<std::vector<unsigned int> > > *roads_stubRefs, std::vector<unsigned int> *tracks_roadRef);
    
    void splitEventAtAM(unsigned int nAM, std::vector<EventCharacteristics> *events);
  
  public: 
    std::array<double, 6> nStubs_layer;
    double nPatterns;
    double nSumMaxSS;
    double nCombinations;
    double nTracks;
    // std::vector<TH1F*> v_h_nStubs;
    
    std::vector<float> stubs_modId_;
    std::vector<float> stubs_r_;
    std::vector<std::vector<std::vector<unsigned int> > > roads_stubRefs_;
    std::vector<unsigned int> tracks_roadRef_;
};
