#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "TRandom3.h"
#include "TH1F.h"
#include "TFile.h"

#include "interface/ComponentRelation.h"
#include "interface/DataSource.h"
#include "interface/StubMapper.h"
#include "interface/AssociativeMemory.h"
#include "interface/HitBuffer.h"

void removeSpaces(std::string &input)
{
  input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
}

int main()
{
  std::vector<ComponentRelation*> componentRelations;
  
  // Read the schematic file and store Component Characteristics
  // and the schematic arrangement between components
  std::ifstream file("Schematic.txt");
  std::string s;
  getline(file, s);
  while (!file.eof())
  {
    getline(file, s);
    std::stringstream ss(s);
    std::string index_s; getline(ss, index_s, '|');
    std::string component_s; getline(ss, component_s, '|');
    
    if (component_s.find("DataSource")!=std::string::npos)
    {
      std::string name_s, outTime_s;
      getline(ss, name_s, '|');
      getline(ss, outTime_s, '|');
      double outTime=atof(outTime_s.c_str());
      removeSpaces(name_s);
      DataSource *ds=new DataSource(name_s, outTime);
      ComponentRelation *compRelation=new ComponentRelation();
      compRelation->extractComponentRelation(s);
      compRelation->comp_=ds;
      componentRelations.push_back(compRelation);
      std::cout<<"Initialized DataSource "<<name_s<<std::endl;
    }
    
    if (component_s.find("StubMapper")!=std::string::npos)
    {
      std::string name_s, delay_s;
      getline(ss, name_s, '|');
      getline(ss, delay_s, '|');
      double delay=atof(delay_s.c_str());
      removeSpaces(name_s);
      StubMapper *sm=new StubMapper(name_s, delay);
      ComponentRelation *compRelation=new ComponentRelation();
      compRelation->extractComponentRelation(s);
      compRelation->comp_=sm;
      componentRelations.push_back(compRelation);
      std::cout<<"Initialized StubMapper "<<name_s<<std::endl;
    }
    
    if (component_s.find("AssociativeMemory")!=std::string::npos)
    {
      std::string name_s, delay_s, inTime_s, outTime_s;
      getline(ss, name_s, '|');
      getline(ss, delay_s, ',');
      getline(ss, inTime_s, ',');
      getline(ss, outTime_s, '|');
      removeSpaces(name_s);
      double delay=atof(delay_s.c_str());
      double inTime=atof(inTime_s.c_str());
      double outTime=atof(outTime_s.c_str());
      AssociativeMemory *am=new AssociativeMemory(name_s, delay, inTime, outTime);
      ComponentRelation *compRelation=new ComponentRelation();
      compRelation->extractComponentRelation(s);
      compRelation->comp_=am;
      componentRelations.push_back(compRelation);
      std::cout<<"Initialized AssociativeMemory "<<name_s<<std::endl;
    }
    
    if (component_s.find("HitBuffer")!=std::string::npos)
    {
      std::string name_s, inTime_s, procTime_s, outTime_s, delay_s;
      getline(ss, name_s, '|');
      getline(ss, inTime_s, ',');
      getline(ss, procTime_s, ',');
      getline(ss, outTime_s, ',');
      getline(ss, delay_s, '|');
      removeSpaces(name_s);
      double inTime=atof(inTime_s.c_str());
      double procTime=atof(procTime_s.c_str());
      double outTime=atof(outTime_s.c_str());
      double delay=atof(delay_s.c_str());
      HitBuffer *hb=new HitBuffer(name_s, inTime, procTime, outTime, delay);
      ComponentRelation *compRelation=new ComponentRelation();
      compRelation->extractComponentRelation(s);
      compRelation->comp_=hb;
      componentRelations.push_back(compRelation);
      std::cout<<"Initialized HitBuffer "<<name_s<<std::endl;
    } 
      
  
  }
  
  // Read ROOT event files and iterate
  
  unsigned int nEvents=1000;
  for (unsigned int i_event=0; i_event<nEvents; ++i_event)
  {
    EventCharacteristics event;
    
    // Load some fake data
    event.nStubs_layer.at(0)=std::max(0, int(r3->Gaus(115, 25)));
    event.nStubs_layer.at(1)=std::max(0, int(r3->Gaus(77, 21)));
    event.nStubs_layer.at(2)=std::max(0, int(r3->Gaus(63, 20)));
    event.nStubs_layer.at(3)=std::max(0, int(r3->Gaus(40, 18)));
    event.nStubs_layer.at(4)=std::max(0, int(r3->Gaus(46, 22)));
    event.nStubs_layer.at(5)=std::max(0, int(r3->Gaus(48, 23)));
    event.nPatterns=std::max(0, r3->Poisson(12));
    event.nOutwords=10;
    
    /*std::cout<<"=== Event === "<<std::endl;
    for (unsigned int i=0; i<6; ++i)
    {
      std::cout<<"event.nStubs_layer.at("<<i<<") = "<<event.nStubs_layer.at(i)<<std::endl;
    }
    std::cout<<"event.nPatterns = "<<event.nPatterns<<std::endl;
    std::cout<<"=== ==="<<std::endl;*/
    
    // iterate over componentRelations
    for (unsigned int i_comp=0; i_comp<componentRelations.size(); ++i_comp)
    {
      ComponentRelation *componentRelation=componentRelations.at(i_comp);
      Component *component=componentRelation->comp_;
      component->setEventCharacteristics(&event);
      component->computeOutputTimes();
      
      // Printout outputs of this component
      // component->printOutputTimes();
      
      // How many outputs does this component have?
      // Connect all of them to specified inputs of the i_comp
      for (unsigned int i_pin=0; i_pin<componentRelation->i_comp_.size(); ++i_pin)
      {
        if (componentRelation->i_comp_.at(i_pin)<componentRelations.size())
        {
          Component *targetComponent=componentRelations.at(componentRelation->i_comp_.at(i_pin))->comp_;
          targetComponent->set_t1in(componentRelation->i_input_.at(i_pin), component->get_t1out(componentRelation->i_output_.at(i_pin)));
          targetComponent->set_t2in(componentRelation->i_input_.at(i_pin), component->get_t2out(componentRelation->i_output_.at(i_pin)));
        }
        else
        {
          std::cout<<"ERROR: Schematic file. Output pins of component "<<component->get_name()<<" refer to non-existent indices in schematic."<<std::endl;
        }
      }
    }
  }
  
  // Draw the histograms
  for (unsigned int i_comp=0; i_comp<componentRelations.size(); ++i_comp)
  {
    Component *component=componentRelations.at(i_comp)->comp_;
    component->writeOutputTimes();
    component->drawOutputTimes();
  }
  
  return 0;
  
}
