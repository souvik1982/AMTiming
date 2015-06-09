#include <iostream>

#include "../interface/DataSource.h"

DataSource::DataSource(std::string name, double outTime)
{
  name_=name;
  outTime_=outTime;
  for (unsigned int i=0; i<6; ++i)
  {
    t1in_.push_back(0);
    t2in_.push_back(0);
    t1out_.push_back(-999);
    t2out_.push_back(-999);
    
    v_h_t1out_.push_back(0);
    v_h_t2out_.push_back(new TH1F(("h_t2out_"+name_+itoa(i)).c_str(), (";DataSource "+name_+" layer "+itoa(i)+" t2out").c_str(), 100, 0, 10000));
  } 
}

bool DataSource::setEventCharacteristics(EventCharacteristics *event)
{
  nStubs_layer_=event->nStubs_layer;
  return true;
}

bool DataSource::computeOutputTimes()
{
  if (outTime_>0)
  {
    for (unsigned int i=0; i<6; ++i)
    {
      if (nStubs_layer_.at(i)!=-999)
      {
        if (t1in_.at(i)!=-999)                                                                       
        {                                                                                            
          t1out_.at(i)=t1in_.at(i);
          t2out_.at(i)=t1in_.at(i)+nStubs_layer_.at(i)*outTime_;
          
          v_h_t2out_.at(i)->Fill(t2out_.at(i));
        }                                                                                            
        else                                                                                         
        {                                                                                            
          std::cout<<"ERROR: DataSource "<<name_<<" has Input Pin t1in["<<i<<"] = "<<t1in_.at(i)<<std::endl;          
          return false;                                                                              
        }
      }
      else
      {
        std::cout<<"ERROR: DataSource "<<name_<<" has Event Characteristic nStubs_layer["<<i<<"] = "<<nStubs_layer_.at(i)<<std::endl;
        return false;
      }                                                                                     
    }
  }
  else
  {
    std::cout<<"ERROR: DataSource "<<name_<<" has Component Characteristic outTime = "<<outTime_<<std::endl;
    return false;
  }
  return true;
}   
    
