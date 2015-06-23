#include <iostream>

#include "../interface/EventCharacteristics.h"

EventCharacteristics::EventCharacteristics(std::vector<float> *stubs_modId, std::vector<float> *stubs_r, std::vector<std::vector<std::vector<unsigned int> > > *roads_stubRefs)
{
  nStubs_layer.at(0)=0;
  nStubs_layer.at(1)=0;
  nStubs_layer.at(2)=0;
  nStubs_layer.at(3)=0;
  nStubs_layer.at(4)=0;
  nStubs_layer.at(5)=0;
  for (unsigned int i=0; i<stubs_modId->size(); ++i)
  {
    if (stubs_r->at(i)>20)
    {
      int n=int(stubs_modId->at(i)/10000);
      if (n==5) ++nStubs_layer.at(0);
      else if (n==6) ++nStubs_layer.at(1);
      else if (n==7) ++nStubs_layer.at(2);
      else if (n==8) ++nStubs_layer.at(3);
      else if (n==9) ++nStubs_layer.at(4);
      else if (n==10) ++nStubs_layer.at(5);
      else std::cout<<"ERROR: stubs_modId = "<<stubs_modId->at(i)<<" doesn't correspond to any layer!"<<std::endl;
    }
  }
  
  nPatterns=roads_stubRefs->size();
  nOutwords=0;
  for (unsigned int i_road=0; i_road<roads_stubRefs->size(); ++i_road)
  {
    int nMaxStubPerSuperstrip=0;
    for (unsigned int i_superstrip=0; i_superstrip<roads_stubRefs->at(i_road).size(); ++i_superstrip)
    {
      unsigned int nStubs=roads_stubRefs->at(i_road).at(i_superstrip).size();
      if (nStubs>nMaxStubPerSuperstrip) nMaxStubPerSuperstrip=nStubs;
    }
    nOutwords+=nMaxStubPerSuperstrip;
  }
}
