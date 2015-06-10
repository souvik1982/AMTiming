#include "Component.h"

class TrackFitter: public Component
{
  public:
  
    TrackFitter(std::string name, double inTime, double outTime, double delay);
    bool setEventCharacteristics(EventCharacteristics*);
    bool computeOutputTimes();
    
  private:
  
    double inTime_, outTime_, delay_;
    double nCombinations_;
    double nTracks_;
};
