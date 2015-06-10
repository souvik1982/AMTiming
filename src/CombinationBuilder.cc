#include <iostream>

#include "../interface/CombinationBuilder.h"

CombinationBuilder::CombinationBuilder(std::string name, double inTime, double outTime, double delay)
{
  name_=name;
  inTime_=inTime;
  outTime_=outTime;
  delay_=delay;
  
  t1in_.push_back(-999);
  t2in_.push_back(-999);
  t1out_.push_back(-999);
  t2out_.push_back(-999);
  
  v_h_t1out_.push_back(new TH1F(("h_t1out_"+name_).c_str(), (";CombinationBuilder "+name_+" t1out").c_str(), 100, 0, 10000));
  v_h_t2out_.push_back(new TH1F(("h_t2out_"+name_).c_str(), (";CombinationBuilder "+name_+" t2out").c_str(), 100, 0, 10000));
}

bool CombinationBuilder::setEventCharacteristics(EventCharacteristics *event)
{
  nPatterns_=event->nPatterns;
  nCombinations_=event->nCombinations;
  return true;
}

bool CombinationBuilder::computeOutputTimes()
{
  if (delay_>0 && inTime_>0 && outTime_>0)
  {
    if (nPatterns_!=-999 && nCombinations_!=-999)
    {
      if (t1in_.at(0)!=-999 && t2in_.at(0)!=-999)
      {
        t1out_.at(0)=t1in_.at(0)+delay_;
        t2out_.at(0)=std::max(t2in_.at(0)+delay_, std::max(t1out_.at(0)+nPatterns_*inTime_, t1out_.at(0)+nCombinations_*outTime_));
        
        v_h_t1out_.at(0)->Fill(t1out_.at(0));
        v_h_t2out_.at(0)->Fill(t2out_.at(0));
      }
      else
      {
        std::cout<<"ERROR: CombinationBuilder "<<name_<<" has input pins t1in = "<<t1in_.at(0)<<" and t2in = "<<t2in_.at(0)<<std::endl;
        return false;
      }
    }
    else
    {
      std::cout<<"ERROR: CombinationBuilder "<<name_<<" has Event Characteristic nPatterns = "<<nPatterns_<<", and nCombinations = "<<nCombinations_<<std::endl;
      return false;
    }
  }
  else
  {
    std::cout<<"ERROR: CombinationBuilder "<<name_<<" has Component Characteristics not set."<<std::endl;
    std::cout<<"       delay = "<<delay_<<std::endl;
    std::cout<<"       inTime = "<<inTime_<<std::endl;
    std::cout<<"       outTime = "<<outTime_<<std::endl;
    std::cout<<"====="<<std::endl;
    return false;
  }
  return true;
}
