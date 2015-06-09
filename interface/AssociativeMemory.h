#include "Component.h"

class AssociativeMemory: public Component
{
  public:
  
    AssociativeMemory(std::string, double, double, double);
    bool setEventCharacteristics(EventCharacteristics*);
    bool computeOutputTimes();
    
  private:
    double delay_, inTime_, outTime_;
    double nPatterns_;
    std::array<double, 6> nStubs_layer_;
};
