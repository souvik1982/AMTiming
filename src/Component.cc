#include <iostream>

#include "../interface/Component.h"

void Component::set_t1in(unsigned int i_in, double value)
{
  if (i_in<t1in_.size())
  {
    t1in_.at(i_in)=value;
  }
  else
  {
    std::cout<<"ERROR: Component "<<name_<<" has no input #"<<i_in<<std::endl;
  }
}

void Component::set_t2in(unsigned int i_in, double value)
{
  if (i_in<t2in_.size())
  {
    t2in_.at(i_in)=value;
  }
  else
  {
    std::cout<<"ERROR: Component "<<name_<<" has no input #"<<i_in<<std::endl;
  }
}

double Component::get_t1out(unsigned int i_out)
{
  if (i_out<t1out_.size())
  {
    return t1out_.at(i_out);
  }
  else
  {
    std::cout<<"ERROR: Component "<<name_<<" has no output #"<<i_out<<std::endl;
    return -99;
  }
}

double Component::get_t2out(unsigned int i_out)
{
  if (i_out<t2out_.size())
  {
    return t2out_.at(i_out);
  }
  else
  {
    std::cout<<"ERROR: Component "<<name_<<" has no output #"<<i_out<<std::endl;
    return -99;
  }
}

std::string Component::get_name()
{
  return name_;
}

void Component::printOutputTimes()
{
  std::cout<<" === Component "<<name_<<" Timing "<<std::endl;
  for (unsigned int i=0; i<t1out_.size(); ++i)
  { 
    std::cout<<"  t1out["<<i<<"] = "<<t1out_.at(i)<<std::endl;
  }
  for (unsigned int i=0; i<t2out_.size(); ++i)
  { 
    std::cout<<"  t2out["<<i<<"] = "<<t2out_.at(i)<<std::endl;
  }
  std::cout<<" === === "<<std::endl;
} 

void Component::clearValues()
{
  for (unsigned int i=0; i<t1in_.size(); ++i)
  {
    t1in_.at(i)=-999;
    t2in_.at(i)=-999;
  }
  for (unsigned int i=0; i<t1out_.size(); ++i)
  {
    t1out_.at(i)=-999;
    t2out_.at(i)=-999;
  }
}
