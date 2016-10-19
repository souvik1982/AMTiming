#include "Component.h"

class TrackFitter: public Component
{
  public:
  
    TrackFitter(std::string name, double frequency, double inCLK, double outCLK, double delayCLK);
    bool computeOutputTimes();
    void writeHistograms();
    
    double getInTime(){return inTime_;}
    
  private:
  
    double inTime_, outTime_, delay_;
    TH1F *h_nCombinations_;
    TH1F *h_nTracks_;
};
