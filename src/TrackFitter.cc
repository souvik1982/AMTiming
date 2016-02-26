#include <iostream>

#include "../interface/TrackFitter.h"

TrackFitter::TrackFitter(std::string name, double inTime, double outTime, double delay)
{
  type_="TrackFitter";
  name_=name;
  inTime_=inTime;
  outTime_=outTime;
  delay_=delay;
  
  t1in_.push_back(-999);
  t2in_.push_back(-999);
  t1out_.push_back(-999);
  t2out_.push_back(-999);
  
  v_h_t1out_.push_back(new TH1F(("h_t1out_"+name_).c_str(), (";TrackFitter "+name_+" t1out").c_str(), 20000, 0, 20000));
  v_h_t2out_.push_back(new TH1F(("h_t2out_"+name_).c_str(), (";TrackFitter "+name_+" t2out").c_str(), 20000, 0, 20000));
  
  h_nCombinations_=new TH1F(("h_nCombinations_"+name_).c_str(), "; nCombinations", 10000, 0, 10000);
  h_nTracks_=new TH1F(("h_nTracks_"+name_).c_str(), "; nTracks", 1000, 0, 1000);

  std::cout<<"LOG: Initialized TrackFitter "<<name_<<" with inTime = "<<inTime_<<" ns, outTime = "<<outTime_<<" ns, delay = "<<delay_<<" ns"<<std::endl;
}

bool TrackFitter::computeOutputTimes()
{
  if (delay_>0 && inTime_>0 && outTime_>0)
  {
    if (event_.nCombinations!=-999 && event_.nTracks!=-999)
    {
      if (t1in_.at(0)!=-999 && t2in_.at(0)!=-999)
      {
        t1out_.at(0)=t1in_.at(0)+delay_;
        t2out_.at(0)=std::max(t2in_.at(0)+delay_, std::max(t1out_.at(0)+(event_.nCombinations+1)*inTime_, t1out_.at(0)+(event_.nTracks+1)*outTime_));
        
        v_h_t1out_.at(0)->Fill(t1out_.at(0));
        v_h_t2out_.at(0)->Fill(t2out_.at(0));
      }
      else
      {
        std::cout<<"ERROR: TrackFitter "<<name_<<" has input pins t1in = "<<t1in_.at(0)<<" and t2in = "<<t2in_.at(0)<<std::endl;
        return false;
      }
    }
    else
    {
      std::cout<<"ERROR: TrackFitter "<<name_<<" has Event Characteristic nCombinations = "<<event_.nCombinations<<", and nTracks = "<<event_.nTracks<<std::endl;
      return false;
    }
  }
  else
  {
    std::cout<<"ERROR: TrackFitter "<<name_<<" has Component Characteristics not set."<<std::endl;
    std::cout<<"       delay = "<<delay_<<std::endl;
    std::cout<<"       inTime = "<<inTime_<<std::endl;
    std::cout<<"       outTime = "<<outTime_<<std::endl;
    std::cout<<"====="<<std::endl;
    return false;
  }
  
  // Fill histograms
  h_nCombinations_->Fill(event_.nCombinations);
  h_nTracks_->Fill(event_.nTracks);
  
  return true;
}

void TrackFitter::writeHistograms()
{
  TFile *file=new TFile((name_+".root").c_str(), "recreate");
  for (unsigned int i=0; i<v_h_t1out_.size(); ++i)
  { 
    if (v_h_t1out_.at(i)!=0) v_h_t1out_.at(i)->Write();
    if (v_h_t2out_.at(i)!=0) v_h_t2out_.at(i)->Write();
  }
  h_nCombinations_->Write();
  h_nTracks_->Write();
  file->Close();
} 

