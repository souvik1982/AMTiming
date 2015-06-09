#include <iostream>

#include "../interface/HitBuffer.h"

HitBuffer::HitBuffer(std::string name, double inTime, double procTime, double outTime, double delay)
{
  name_=name;
  inTime_=inTime;
  procTime_=procTime;
  outTime_=outTime;
  delay_=delay;
  for (unsigned int i=0; i<6; ++i)
  {
    t1in_.push_back(-999);
    t2in_.push_back(-999);
  }
  t1in_.push_back(-999);
  t2in_.push_back(-999);
  t1out_.push_back(-999);
  t2out_.push_back(-999);
  
  v_h_t1out_.push_back(new TH1F(("h_t1out_"+name_).c_str(), (";HitBuffer "+name_+" t1out").c_str(), 100, 0, 10000));
  v_h_t2out_.push_back(new TH1F(("h_t2out_"+name_).c_str(), (";HitBuffer "+name_+" t2out").c_str(), 100, 0, 10000));
}

bool HitBuffer::setEventCharacteristics(EventCharacteristics *event)
{
  nStubs_layer_=event->nStubs_layer;
  nPatterns_=event->nPatterns;
  nOutwords_=event->nOutwords;
  return true;
}

bool HitBuffer::computeOutputTimes()
{
  if (delay_>0 && inTime_>0 && procTime_>0 && outTime_>0)
  {
    if (t1in_.at(6)!=-999 && t2in_.at(6)!=-999)
    {
      double maxTime=-999;
      for (unsigned int i=0; i<6; ++i)
      {
        if (nStubs_layer_.at(i)!=-999)
        {
          double maxTime_layer=std::max(t1in_.at(6), std::max(t1in_.at(i)+nStubs_layer_.at(i)*inTime_, t2in_.at(i)));
          if (maxTime_layer>maxTime) maxTime=maxTime_layer;
        }
        else
        {
          std::cout<<"ERROR: HitBuffer "<<name_<<" has Event Characteristic nStubs_layer.at("<<i<<") = "<<nStubs_layer_.at(i)<<std::endl;
          return false;
        }
      }
      t1out_.at(0)=maxTime+delay_;
      t2out_.at(0)=std::max(t2in_.at(6)+delay_, std::max(t1out_.at(0)+nPatterns_*procTime_, nOutwords_*outTime_));
      
      v_h_t1out_.at(0)->Fill(t1out_.at(0));
      v_h_t2out_.at(0)->Fill(t2out_.at(0));
    }
    else
    {
      std::cout<<"ERROR: HitBuffer "<<name_<<" has pin t1in_.at(6) = "<<t1in_.at(6)<<std::endl;
    }
  }
  else
  {
    std::cout<<"ERROR: HitBuffer "<<name_<<" has Component Characteristics not set."<<std::endl;
    std::cout<<"       delay = "<<delay_<<std::endl;
    std::cout<<"       inTime = "<<inTime_<<std::endl;
    std::cout<<"       procTime = "<<procTime_<<std::endl;
    std::cout<<"       outTime = "<<outTime_<<std::endl;
    std::cout<<"====="<<std::endl;
  }
  return true;
}
 
