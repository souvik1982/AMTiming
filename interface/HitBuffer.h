#include "Component.h"

class HitBuffer: public Component
{
  public:
  
    HitBuffer(std::string name, double frequency, double inCLK, double procCLK, double outCLK, double delayCLK);
    bool computeOutputTimes();
    void writeHistograms();
    
    double getOutTime(){return outTime_;}
    
  private:
  
    double inTime_, procTime_, outTime_, delay_;
    std::vector<TH1F*> v_h_nStubs_;
    TH1F *h_nPatterns_;
    TH1F *h_nOutwords_;
};
    
    
