#include "Component.h"

class AssociativeMemory: public Component
{
  public:
  
    AssociativeMemory(std::string, double, double, double);
    bool setEventCharacteristics(EventCharacteristics*);
    bool computeOutputTimes();
    void writeHistograms();
    
    double getInTime(){return inTime_;}
    
  private:
    double delay_, inTime_, outTime_;
    double nPatterns_;
    std::array<double, 6> nStubs_layer_;
    std::vector<TH1F*> v_h_nStubs_;
    TH1F *h_nPatterns_;
    
    std::vector<TH1F*> v_h_t1out_term1_; // The t2in from the SM for layer k
    std::vector<TH1F*> v_h_t1out_term2_; // The t1in from the SM + nStubs*inTime for layer k
    
};
