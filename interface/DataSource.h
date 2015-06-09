#include <array>

#include "Component.h"

class DataSource: public Component
{
  public:
    
    DataSource(std::string name, double outTime);
    bool setEventCharacteristics(EventCharacteristics*);
    bool computeOutputTimes();
  
  private:
    std::array<double, 6> nStubs_layer_;
    double outTime_;
};
