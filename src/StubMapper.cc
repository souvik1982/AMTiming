#include <iostream>

#include "../interface/StubMapper.h"

StubMapper::StubMapper(std::string name, double delay)
{
  name_=name;
  delay_=delay;
  for (unsigned int i=0; i<6; ++i)
  {
    t1in_.push_back(-999);
    t2in_.push_back(-999);
    t1out_.push_back(-999);
    t2out_.push_back(-999);
    
    v_h_t1out_.push_back(new TH1F(("h_t1out_"+name_+itoa(i)).c_str(), (";StubMapper "+name_+" layer "+itoa(i)+" t1out").c_str(), 100, 0, 10000));
    v_h_t2out_.push_back(new TH1F(("h_t2out_"+name_+itoa(i)).c_str(), (";StubMapper "+name_+" layer "+itoa(i)+" t2out").c_str(), 100, 0, 10000));
  } 
}

bool StubMapper::computeOutputTimes()
{
  if (delay_<0)
  {
    std::cout<<"ERROR: StubMapper "<<name_<<" has delay = "<<delay_<<std::endl;
    return false;
  }
  
  for (unsigned int i=0; i<6; ++i)
  {
    if (t1in_.at(i)!=-999 && t2in_.at(i)!=-999)
    {
      t1out_.at(i)=t1in_.at(i)+delay_;
      t2out_.at(i)=t2in_.at(i)+delay_;
      
      v_h_t1out_.at(i)->Fill(t1out_.at(i));
      v_h_t2out_.at(i)->Fill(t2out_.at(i));
    }
    else
    {
      std::cout<<"ERROR: StubMapper "<<name_<<" has not been set. t1in_.at("<<i<<") = "<<t1in_.at(i)<<", t2in_.at("<<i<<") = "<<t2in_.at(i)<<std::endl;
      return false;
    }
  }
  return true;
}
   
bool StubMapper::setEventCharacteristics(EventCharacteristics *event)
{
  return true;
}
