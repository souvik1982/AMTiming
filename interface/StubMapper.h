#include "Component.h"

class StubMapper: public Component
{
  public:
  
    StubMapper(std::string name, double frequency, double delayCLK);
    // bool setEventCharacteristics(EventCharacteristics*);
    bool computeOutputTimes();
    void writeHistograms();
    
  private:
    double delay_;
};
