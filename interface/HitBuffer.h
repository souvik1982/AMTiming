#include "Component.h"

class HitBuffer: public Component
{
  public:
  
    HitBuffer(std::string name, double inTime, double procTime, double outTime, double delay);
    bool setEventCharacteristics(EventCharacteristics*);
    bool computeOutputTimes();
    void writeHistograms();
    
  private:
  
    double inTime_, procTime_, outTime_, delay_;
    double nPatterns_;
    double nOutwords_;
    std::array<double, 6> nStubs_layer_;
    std::vector<TH1F*> v_h_nStubs_;
    TH1F *h_nPatterns_;
    TH1F *h_nOutwords_;
};
    
    
