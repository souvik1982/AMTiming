#include <iostream>

#include "../interface/AssociativeMemory.h"

AssociativeMemory::AssociativeMemory(std::string name, double delay, double inTime, double outTime)
{
  name_=name;
  delay_=delay;
  inTime_=inTime;
  outTime_=outTime;
  for (unsigned int i=0; i<6; ++i)
  {
    t1in_.push_back(-999);
    t2in_.push_back(-999);
  }
  t1out_.push_back(-999);
  t2out_.push_back(-999);
  
  v_h_t1out_.push_back(new TH1F(("h_t1out_"+name_).c_str(), (";AssociativeMemory "+name_+" t1out").c_str(), 100, 0, 10000));
  v_h_t2out_.push_back(new TH1F(("h_t2out_"+name_).c_str(), (";AssociativeMemory "+name_+" t2out").c_str(), 100, 0, 10000));  
}

bool AssociativeMemory::setEventCharacteristics(EventCharacteristics *event)
{
  nStubs_layer_=event->nStubs_layer;
  nPatterns_=event->nPatterns;
  return true;
}

bool AssociativeMemory::computeOutputTimes()
{
  if (delay_>0 && inTime_>0 && outTime_>0)
  {
    double maxTime=-999;
    for (unsigned int i=0; i<6; ++i)
    {
      if (nStubs_layer_.at(i)!=-999)
      {
        if (t1in_.at(i)!=-999 && t2in_.at(i)!=-999)
        {
          double maxTime_layer=std::max(t2in_.at(i), t1in_.at(i)+nStubs_layer_.at(i)*inTime_);
          if (maxTime_layer>maxTime) maxTime=maxTime_layer;
        }
        else
        {
          std::cout<<"ERROR: AssociativeMemory "<<name_<<" has input pins not set. "<<std::endl;
          std::cout<<"       t1in("<<i<<") = "<<t1in_.at(i)<<std::endl;
          std::cout<<"       t2in("<<i<<") = "<<t2in_.at(i)<<std::endl;
          std::cout<<"====="<<std::endl;
        }
      }
      else
      {
        std::cout<<"ERROR: AssociativeMemory "<<name_<<" has Event Characteristic nStubs_layer.at("<<i<<") = "<<nStubs_layer_.at(i)<<std::endl;
        return false;
      }
    }
    t1out_.at(0)=maxTime+delay_;
    v_h_t1out_.at(0)->Fill(t1out_.at(0));
    if (nPatterns_!=-999)
    {
      t2out_.at(0)=t1out_.at(0)+nPatterns_*outTime_;
      v_h_t2out_.at(0)->Fill(t2out_.at(0));
    }
    else
    {
      std::cout<<"ERROR: AssociativeMemory "<<name_<<" has Event Characteristic nPatterns = "<<nPatterns_<<std::endl;
      return false;
    }
  }
  else
  {
    std::cout<<"ERROR: AssociativeMemory "<<name_<<" has Component Characteristics not set."<<std::endl;
    std::cout<<"       delay = "<<delay_<<std::endl;
    std::cout<<"       inTime = "<<inTime_<<std::endl;
    std::cout<<"       outTime = "<<outTime_<<std::endl;
    std::cout<<"====="<<std::endl;
    return false;
  }
  return true;
}

