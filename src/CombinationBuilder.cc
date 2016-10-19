#include <iostream>

#include "../interface/CombinationBuilder.h"

CombinationBuilder::CombinationBuilder(std::string name, double frequency, double inCLK, double outCLK, double delayCLK)
{
  type_="CombinationBuilder";
  name_=name;
  frequency_=frequency;
  inTime_=inCLK/frequency*1000.;
  outTime_=outCLK/frequency*1000.;
  delay_=delayCLK/frequency*1000.;
  
  t1in_.push_back(-999);
  t2in_.push_back(-999);
  t1out_.push_back(-999);
  t2out_.push_back(-999);
  
  TH1F *h_t1out=new TH1F(("h_t1out_"+name_).c_str(), (";CombinationBuilder "+name_+" t1out").c_str(), 10, 1, 0);
  TH1F *h_t2out=new TH1F(("h_t2out_"+name_).c_str(), (";CombinationBuilder "+name_+" t2out").c_str(), 10, 1, 0);
  
  h_t1out->SetCanExtend(TH1::kAllAxes);
  h_t2out->SetCanExtend(TH1::kAllAxes);
  
  v_h_t1out_.push_back(h_t1out);
  v_h_t2out_.push_back(h_t2out);
  
  h_nOutwords_=new TH1F(("h_nOutwords_"+name_).c_str(), "; nOutwords", 10, 1, 0);
  h_nCombinations_=new TH1F(("h_nCombinations_"+name_).c_str(), "; nCombinations", 10, 1, 0);
  
  h_nOutwords_->SetCanExtend(TH1::kAllAxes);
  h_nCombinations_->SetCanExtend(TH1::kAllAxes);
  
  std::cout<<"LOG: Initialized CombinationBuilder "<<name_<<" with operating frequency = "<<frequency<<", inCLK = "<<inCLK<<", outCLK = "<<outCLK<<", delayCLK = "<<delayCLK<<std::endl;
}

bool CombinationBuilder::computeOutputTimes()
{
  if (delay_>0 && inTime_>0 && outTime_>0)
  {
    if (event_.nSumMaxSS!=-999 && event_.nCombinations!=-999)
    {
      if (t1in_.at(0)!=-999 && t2in_.at(0)!=-999)
      {
        t1out_.at(0)=t1in_.at(0)+delay_;
        t2out_.at(0)=std::max(t2in_.at(0)+delay_, t1out_.at(0)+std::max(event_.nPatterns*inTime_, event_.nCombinations*outTime_));
        
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
      std::cout<<"ERROR: CombinationBuilder "<<name_<<" has Event Characteristic nOutwords = "<<event_.nSumMaxSS<<", and nCombinations = "<<event_.nCombinations<<std::endl;
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
  
  // Fill histograms
  h_nOutwords_->Fill(event_.nSumMaxSS);
  h_nCombinations_->Fill(event_.nCombinations);
  
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
