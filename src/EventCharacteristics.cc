#include <TRandom3.h>
#include <iostream>
#include <map>
#include <algorithm>

#include "../interface/EventCharacteristics.h"

TRandom3 *r3=new TRandom3();
unsigned int throwIn(unsigned int n)
{
  return int(r3->Uniform()*double(n));
}

int stubLayer(float r, float modID)
{
  int n=-1;
  if (r>20) n=int(modID/10000);
  return n;
}   

EventCharacteristics::EventCharacteristics()
{
  nStubs_layer.at(0)=0;
  nStubs_layer.at(1)=0;
  nStubs_layer.at(2)=0;
  nStubs_layer.at(3)=0;
  nStubs_layer.at(4)=0;
  nStubs_layer.at(5)=0;
  nPatterns=0;
  nSumMaxSS=0;
  nCombinations=0;
  nTracks=0;
} 

EventCharacteristics::EventCharacteristics(std::vector<float> *stubs_modId, std::vector<float> *stubs_r, std::vector<std::vector<std::vector<unsigned int> > > *roads_stubRefs, std::vector<unsigned int> *tracks_roadRef)
{
  stubs_modId_=*stubs_modId;
  stubs_r_=*stubs_r;
  roads_stubRefs_=*roads_stubRefs;
  tracks_roadRef_=*tracks_roadRef;
  
  nStubs_layer.at(0)=0;
  nStubs_layer.at(1)=0;
  nStubs_layer.at(2)=0;
  nStubs_layer.at(3)=0;
  nStubs_layer.at(4)=0;
  nStubs_layer.at(5)=0;
  for (unsigned int i=0; i<stubs_modId_.size(); ++i)
  {
    int n=stubLayer(stubs_r_.at(i), stubs_modId_.at(i));
    if (n!=-1)
    {
      if (n==5) ++nStubs_layer.at(0);
      else if (n==6) ++nStubs_layer.at(1);
      else if (n==7) ++nStubs_layer.at(2);
      else if (n==8) ++nStubs_layer.at(3);
      else if (n==9) ++nStubs_layer.at(4);
      else if (n==10) ++nStubs_layer.at(5);
      else std::cout<<"ERROR: stubs_modId = "<<stubs_modId_.at(i)<<" doesn't correspond to any layer!"<<std::endl;
    }
  }
  
  nPatterns=roads_stubRefs_.size();
  nSumMaxSS=0;
  nCombinations=0;
  
  std::vector<std::map<int, int> > v_firedSSPerLayer(6); // <SSID, number of stubs>(layer)
  
  std::cout<<" === NEW EVENT === "<<std::endl;
  for (unsigned int i_road=0; i_road<roads_stubRefs_.size(); ++i_road)
  {
    // int nMaxStubPerSuperstrip=0;
    // double nCombRoad=1;
    
    std::cout<<"Road # "<<i_road<<std::endl;
    for (unsigned int i_superstrip=0; i_superstrip<roads_stubRefs_.at(i_road).size(); ++i_superstrip)
    {
      unsigned int nStubs=roads_stubRefs_.at(i_road).at(i_superstrip).size();
      for (unsigned int i_stub=0; i_stub<nStubs; ++i_stub)
      {
        int stubIndex=roads_stubRefs_.at(i_road).at(i_superstrip).at(i_stub);
        int n=stubLayer(stubs_r_.at(stubIndex), stubs_modId_.at(stubIndex));
        std::cout<<" layer = "<<n<<std::endl;
        if (n>=5 and n<11) ++v_firedSSPerLayer.at(n-5)[i_superstrip];
      }
    }
    
    for (unsigned int i_v_firedSSPerLayer=0; i_v_firedSSPerLayer<v_firedSSPerLayer.size(); ++i_v_firedSSPerLayer)
    {
      std::cout<<" Layer "<<i_v_firedSSPerLayer<<" fired "<<v_firedSSPerLayer.at(i_v_firedSSPerLayer).size()<<" SS"<<std::endl;
    }
  }
  std::cout<<" === === "<<std::endl;
    
    
    /*for (unsigned int i_superstrip=0; i_superstrip<roads_stubRefs_.at(i_road).size(); ++i_superstrip)
    {
      unsigned int nStubs=roads_stubRefs_.at(i_road).at(i_superstrip).size();
      if (nStubs>nMaxStubPerSuperstrip) nMaxStubPerSuperstrip=nStubs;
      if (nStubs>0) nCombRoad*=double(nStubs);
    }
    nOutwords+=nMaxStubPerSuperstrip;*/
    // nCombinations+=nCombRoad;
    // For bypass
    // nOutwords+=1;
    // nCombinations+=1;
  //}
  
  // nTracks=tracks_roadRef_.size();
  // For bypass
  nTracks=std::min(nCombinations, double(tracks_roadRef_.size()));
}

void EventCharacteristics::splitEventAtAM(unsigned int nAM, std::vector<EventCharacteristics> *events)
{
  std::vector<std::vector<std::vector<std::vector<unsigned int> > > > event_Roads_stubRefs(nAM);
  std::vector<std::vector<unsigned int> > event_Tracks_roadRef(nAM);
  int nMatchedTracks=0;
  for (unsigned int i_road=0; i_road<roads_stubRefs_.size(); ++i_road)
  {
    int i_AM=throwIn(nAM);
    event_Roads_stubRefs.at(i_AM).push_back(roads_stubRefs_.at(i_road));
    
    // Which tracks point to this road?
    bool foundTrack=false;
    for (unsigned int i_track=0; i_track<tracks_roadRef_.size(); ++i_track)
    {
      if (tracks_roadRef_.at(i_track)==i_road) 
      {
        event_Tracks_roadRef.at(i_AM).push_back(tracks_roadRef_.at(i_track));
        foundTrack=true;
        ++nMatchedTracks;
      }
    }
    // if (foundTrack==false) std::cout<<"WARNING: Road "<<i_road<<" had no tracks associated with it."<<std::endl;
    
  }
  if (nMatchedTracks!=tracks_roadRef_.size()) std::cout<<"ERROR: There are tracks not matched to roads."<<std::endl;
  
  for (unsigned int i_AM=0; i_AM<nAM; ++i_AM)
  {
    EventCharacteristics event(&(stubs_modId_), &(stubs_r_), &(event_Roads_stubRefs.at(i_AM)), &(event_Tracks_roadRef.at(i_AM)));
    events->push_back(event);
  }
}
    
    
  
  
  
  






