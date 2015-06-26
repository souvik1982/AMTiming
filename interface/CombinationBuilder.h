#include "Component.h"

class CombinationBuilder: public Component
{
  public:
  
    CombinationBuilder(std::string name, double inTime, double outTime, double delay);
    bool setEventCharacteristics(EventCharacteristics*);
    bool computeOutputTimes();
    void writeHistograms();
    
  private:
  
    double inTime_, outTime_, delay_;
    double nPatterns_;
    double nCombinations_;
    TH1F *h_nPatterns_;
    TH1F *h_nCombinations_;
};
