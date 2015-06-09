#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

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
  //TFile *file=TFile("Event.root");
  
  unsigned int nEvents=1;
  for (unsigned int i_event=0; i_event<nEvents; ++i_event)
  {
    EventCharacteristics event;
    event.nStubs_layer.at(0)=11;
    event.nStubs_layer.at(1)=12;
    event.nStubs_layer.at(2)=13;
    event.nStubs_layer.at(3)=14;
    event.nStubs_layer.at(4)=15;
    event.nStubs_layer.at(5)=16;
    event.nPatterns=20;
    
    
    // iterate over componentRelations
    std::cout<<"componentRelations.size() = "<<componentRelations.size()<<std::endl;
    for (unsigned int i_comp=0; i_comp<componentRelations.size(); ++i_comp)
    {
      ComponentRelation *componentRelation=componentRelations.at(i_comp);
      Component *component=componentRelation->comp_;
      component->setEventCharacteristics(&event);
      component->computeOutputTimes();
      
      // Printout outputs of this component
      component->printOutputTimes();
      
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
  
  return 0;
  
}
