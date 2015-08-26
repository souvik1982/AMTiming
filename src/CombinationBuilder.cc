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
  
  v_h_t1out_.push_back(new TH1F(("h_t1out_"+name_).c_str(), (";CombinationBuilder "+name_+" t1out").c_str(), 20000, 0, 20000));
  v_h_t2out_.push_back(new TH1F(("h_t2out_"+name_).c_str(), (";CombinationBuilder "+name_+" t2out").c_str(), 20000, 0, 20000));
  
  h_nOutwords_=new TH1F(("h_nOutwords_"+name_).c_str(), "; nOutwords", 10000, 0, 10000);
  h_nCombinations_=new TH1F(("h_nCombinations_"+name_).c_str(), "; nCombinations", 10000, 0, 10000);
}

bool CombinationBuilder::setEventCharacteristics(EventCharacteristics *event)
{
  nOutwords_=event->nOutwords;
  nCombinations_=event->nCombinations;
  h_nOutwords_->Fill(nOutwords_);
  h_nCombinations_->Fill(nCombinations_);
  return true;
}

bool CombinationBuilder::computeOutputTimes()
{
  if (delay_>0 && inTime_>0 && outTime_>0)
  {
    if (nOutwords_!=-999 && nCombinations_!=-999)
    {
      if (t1in_.at(0)!=-999 && t2in_.at(0)!=-999)
      {
        t1out_.at(0)=t1in_.at(0)+delay_;
        t2out_.at(0)=std::max(t2in_.at(0)+delay_, std::max(t1out_.at(0)+nOutwords_*inTime_, t1out_.at(0)+(nCombinations_+1)*outTime_));
        
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
      std::cout<<"ERROR: CombinationBuilder "<<name_<<" has Event Characteristic nOutwords = "<<nOutwords_<<", and nCombinations = "<<nCombinations_<<std::endl;
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

void CombinationBuilder::writeHistograms()
{
  TFile *file=new TFile((name_+".root").c_str(), "recreate");
  for (unsigned int i=0; i<v_h_t1out_.size(); ++i)
  { 
    if (v_h_t1out_.at(i)!=0) v_h_t1out_.at(i)->Write();
    if (v_h_t2out_.at(i)!=0) v_h_t2out_.at(i)->Write();
  }
  h_nOutwords_->Write();
  h_nCombinations_->Write();
  file->Close();
} 
