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
    
    v_h_nStubs_.push_back(new TH1F(("h_nStubs_"+name_+"_"+itoa(i)).c_str(), ("; nStubs "+name_+" layer "+itoa(i)+" t2out").c_str(), 1000, 0, 1000));
  
    v_h_t1out_term1_.push_back(new TH1F(("h_t1out_term1_"+name_+"_"+itoa(i)).c_str(), ("; AssociativeMemory "+name_+" t1out term1").c_str(), 10000, 0, 10000));
    v_h_t1out_term2_.push_back(new TH1F(("h_t1out_term2_"+name_+"_"+itoa(i)).c_str(), ("; AssociativeMemory "+name_+" t1out term2").c_str(), 10000, 0, 10000));
  }
  t1out_.push_back(-999);
  t2out_.push_back(-999);
  
  v_h_t1out_.push_back(new TH1F(("h_t1out_"+name_).c_str(), (";AssociativeMemory "+name_+" t1out").c_str(), 10000, 0, 10000));
  v_h_t2out_.push_back(new TH1F(("h_t2out_"+name_).c_str(), (";AssociativeMemory "+name_+" t2out").c_str(), 10000, 0, 10000));
   
  h_nPatterns_=new TH1F(("h_nPatterns_"+name_).c_str(), "; nPatterns", 1000, 0, 1000);
}

bool AssociativeMemory::setEventCharacteristics(EventCharacteristics *event)
{
  nStubs_layer_=event->nStubs_layer;
  nPatterns_=event->nPatterns;
  for (unsigned int i=0; i<v_h_nStubs_.size(); ++i)
  {
    v_h_nStubs_.at(i)->Fill(nStubs_layer_.at(i));
  }
  h_nPatterns_->Fill(nPatterns_);
  return true;
}

bool AssociativeMemory::computeOutputTimes()
{
  if (delay_>0 && inTime_>0 && outTime_>0)
  {
    double maxTime=-999;
    unsigned int i_max_layer=-999;
    unsigned int i_term=-999;
    for (unsigned int i=0; i<6; ++i)
    {
      if (nStubs_layer_.at(i)!=-999)
      {
        if (t1in_.at(i)!=-999 && t2in_.at(i)!=-999)
        {
          double term1=t2in_.at(i);
          double term2=t1in_.at(i)+(nStubs_layer_.at(i)+1)*inTime_;
          double maxTime_layer=std::max(term1, term2);
          if (maxTime_layer>maxTime) 
          {
            maxTime=maxTime_layer;
            i_max_layer=i;
            i_term=(term1>term2)?1:2;
          }
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
    if (i_term==1)
    {
      v_h_t1out_term1_.at(i_max_layer)->Fill(t1out_.at(0));
    }
    else if (i_term==2)
    {
      v_h_t1out_term2_.at(i_max_layer)->Fill(t1out_.at(0));
    }
    else
    {
      std::cout<<"ERROR: Impossible i_term = "<<i_term<<std::endl;
    } 
    if (nPatterns_!=-999)
    {
      t2out_.at(0)=t1out_.at(0)+(nPatterns_+1)*outTime_;
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

void AssociativeMemory::writeHistograms()
{
  TFile *file=new TFile((name_+".root").c_str(), "recreate");
  if (v_h_t1out_.at(0)!=0) v_h_t1out_.at(0)->Write();
  if (v_h_t2out_.at(0)!=0) v_h_t2out_.at(0)->Write();
  for (unsigned int i=0; i<6; ++i)
  {
    if (v_h_nStubs_.at(i)!=0) v_h_nStubs_.at(i)->Write();
    if (v_h_t1out_term1_.at(i)!=0) v_h_t1out_term1_.at(i)->Write();
    if (v_h_t1out_term2_.at(i)!=0) v_h_t1out_term2_.at(i)->Write();
  }
  h_nPatterns_->Write();
  file->Close();
} 

