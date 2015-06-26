#include "Component.h"

class TrackFitter: public Component
{
  public:
  
    TrackFitter(std::string name, double inTime, double outTime, double delay);
    bool setEventCharacteristics(EventCharacteristics*);
    bool computeOutputTimes();
    void writeHistograms();
    
  private:
  
    double inTime_, outTime_, delay_;
    double nCombinations_;
    double nTracks_;
    TH1F *h_nCombinations_;
    TH1F *h_nTracks_;
};
