#include "Component.h"

class TrackFitter: public Component
{
  public:
  
    TrackFitter(std::string name, double inTime, double outTime, double delay);
    bool computeOutputTimes();
    void writeHistograms();
    
    double getInTime(){return inTime_;}
    
  private:
  
    double inTime_, outTime_, delay_;
    TH1F *h_nCombinations_;
    TH1F *h_nTracks_;
};
