#include "Component.h"

class CombinationBuilder: public Component
{
  public:
  
    CombinationBuilder(std::string name, double inTime, double outTime, double delay);
    bool setEventCharacteristics(EventCharacteristics*);
    bool computeOutputTimes();
    void writeHistograms();
    
    double getInTime(){return inTime_;}
    double getOutTime(){return outTime_;}
    
  private:
  
    double inTime_, outTime_, delay_;
    double nOutwords_;
    double nCombinations_;
    TH1F *h_nOutwords_;
    TH1F *h_nCombinations_;
};
