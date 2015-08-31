#include <array>

#include "Component.h"

class DataSource: public Component
{
  public:
    
    DataSource(std::string name, double outTime);
    // bool setEventCharacteristics(EventCharacteristics*);
    bool computeOutputTimes();
    void writeHistograms();
    
    double getOutTime(){return outTime_;}
  
  private:
    double outTime_;
    std::vector<TH1F*> v_h_nStubs_;
};
