#include <iostream>

#include "../interface/HitBuffer.h"

HitBuffer::HitBuffer(std::string name, double frequency, double inCLK, double procCLK, double outCLK, double delayCLK)
{
  type_="HitBuffer";
  name_=name;
  frequency_=frequency;
  inTime_=inCLK/frequency*1000.;
  procTime_=procCLK/frequency*1000.;
  outTime_=outCLK/frequency*1000.;
  delay_=delayCLK/frequency*1000.;
  for (unsigned int i=0; i<6; ++i)
  {
    t1in_.push_back(-999);
    t2in_.push_back(-999);
    
    TH1F *h_nStubs=new TH1F(("h_nStubs_"+name_+"_"+itoa(i)).c_str(), ("; nStubs "+name_+" layer "+itoa(i)+" t2out").c_str(), 10, 1, 0);
    h_nStubs->SetCanExtend(TH1::kAllAxes);
    v_h_nStubs_.push_back(h_nStubs);
  }
  t1in_.push_back(-999);
  t2in_.push_back(-999);
  t1out_.push_back(-999);
  t2out_.push_back(-999);
  
  TH1F *h_t1out=new TH1F(("h_t1out_"+name_).c_str(), (";HitBuffer "+name_+" t1out").c_str(), 10, 1, 0);
  TH1F *h_t2out=new TH1F(("h_t2out_"+name_).c_str(), (";HitBuffer "+name_+" t2out").c_str(), 10, 1, 0);
  
  h_t1out->SetCanExtend(TH1::kAllAxes);
  h_t2out->SetCanExtend(TH1::kAllAxes);
  
  v_h_t1out_.push_back(h_t1out);
  v_h_t2out_.push_back(h_t2out);
  
  h_nPatterns_=new TH1F(("h_nPatterns_"+name_).c_str(), "; nPatterns", 10, 1, 0);
  h_nOutwords_=new TH1F(("h_nOutwords_"+name_).c_str(), "; nWords", 10, 1, 0);
  
  h_nPatterns_->SetCanExtend(TH1::kAllAxes);
  h_nOutwords_->SetCanExtend(TH1::kAllAxes);
  
  std::cout<<"LOG: Initialized HitBuffer "<<name_<<" with operating frequency = "<<frequency<<", inCLK = "<<inCLK<<", procCLK = "<<procCLK<<", outCLK = "<<outCLK<<", delayCLK = "<<delayCLK<<std::endl;
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
        if (event_.nStubs_layer.at(i)!=-999)
        {
          double maxTime_layer=std::max(t1in_.at(6), std::max(t1in_.at(i)+event_.nStubs_layer.at(i)*inTime_, t2in_.at(i)));
          if (maxTime_layer>maxTime) maxTime=maxTime_layer;
        }
        else
        {
          std::cout<<"ERROR: HitBuffer "<<name_<<" has Event Characteristic event_.nStubs_layer.at("<<i<<") = "<<event_.nStubs_layer.at(i)<<std::endl;
          return false;
        }
      }
      t1out_.at(0)=maxTime+delay_;
      t2out_.at(0)=std::max(t2in_.at(6)+delay_, t1out_.at(0)+std::max(0., event_.nPatterns-1)*outTime_);
      
      v_h_t1out_.at(0)->Fill(t1out_.at(0));
      v_h_t2out_.at(0)->Fill(t2out_.at(0));
    }
    else
    {
      std::cout<<"ERROR: HitBuffer "<<name_<<" has pin t1in_.at(6) = "<<t1in_.at(6)<<std::endl;
      return false;
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
    return false;
  }
  
  // Fill histograms
  for (unsigned int i=0; i<v_h_nStubs_.size(); ++i)
  {
    v_h_nStubs_.at(i)->Fill(event_.nStubs_layer.at(i));
  }
  h_nPatterns_->Fill(event_.nPatterns);
  h_nOutwords_->Fill(event_.nSumMaxSS);
  
  return true;
}

void HitBuffer::writeHistograms()
{
  TFile *file=new TFile((name_+".root").c_str(), "recreate");
  for (unsigned int i=0; i<v_h_t1out_.size(); ++i)
  { 
    if (v_h_t1out_.at(i)!=0) v_h_t1out_.at(i)->Write();
    if (v_h_t2out_.at(i)!=0) v_h_t2out_.at(i)->Write();
    if (v_h_nStubs_.at(i)!=0) v_h_nStubs_.at(i)->Write();
  }
  h_nPatterns_->Write();
  h_nOutwords_->Write();
  file->Close();
} 
 
