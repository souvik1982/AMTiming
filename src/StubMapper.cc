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
