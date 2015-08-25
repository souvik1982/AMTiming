#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "TRandom3.h"
#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"

#include "interface/ComponentRelation.h"
#include "interface/DataSource.h"
#include "interface/StubMapper.h"
#include "interface/AssociativeMemory.h"
#include "interface/HitBuffer.h"
#include "interface/CombinationBuilder.h"
#include "interface/TrackFitter.h"
#include "interface/loader.h"
#include "interface/CommandLineArguments.h"

void removeSpaces(std::string &input)
{
  input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
}

int main(int argc, char *argv[])
{
  std::string schematicFilename="Schematic.txt";
  std::string inputDir;
  std::string inputFile="tracks.root";
  
  // Get command line arguments
  std::map<std::string, std::string> cmdMap=commandLineArguments(argc, argv);
  if (cmdMap.find("-schematic")!=cmdMap.end()) schematicFilename=cmdMap["-schematic"];
  if (cmdMap.find("-inputDir")!=cmdMap.end())  inputDir=cmdMap["-inputDir"];
  if (cmdMap.find("-inputFile")!=cmdMap.end()) inputFile=cmdMap["-inputFile"];
  
  std::vector<ComponentRelation*> componentRelations;
  
  // Read the schematic file and store Component Characteristics
  // and the schematic arrangement between components
  std::ifstream file(schematicFilename.c_str());
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
    
    if (component_s.find("CombinationBuilder")!=std::string::npos)
    {
      std::string name_s, inTime_s, outTime_s, delay_s;
      getline(ss, name_s, '|');
      getline(ss, inTime_s, ',');
      getline(ss, outTime_s, ',');
      getline(ss, delay_s, '|');
      removeSpaces(name_s);
      double inTime=atof(inTime_s.c_str());
      double outTime=atof(outTime_s.c_str());
      double delay=atof(delay_s.c_str());
      CombinationBuilder *cb=new CombinationBuilder(name_s, inTime, outTime, delay);
      ComponentRelation *compRelation=new ComponentRelation();
      compRelation->extractComponentRelation(s);
      compRelation->comp_=cb;
      componentRelations.push_back(compRelation);
      std::cout<<"Initialized CombinationBuilder "<<name_s<<std::endl;
    }
    
    if (component_s.find("TrackFitter")!=std::string::npos)
    {
      std::string name_s, inTime_s, outTime_s, delay_s;
      getline(ss, name_s, '|');
      getline(ss, inTime_s, ',');
      getline(ss, outTime_s, ',');
      getline(ss, delay_s, '|');
      removeSpaces(name_s);
      double inTime=atof(inTime_s.c_str());
      double outTime=atof(outTime_s.c_str());
      double delay=atof(delay_s.c_str());
      TrackFitter *tf=new TrackFitter(name_s, inTime, outTime, delay);
      ComponentRelation *compRelation=new ComponentRelation();
      compRelation->extractComponentRelation(s);
      compRelation->comp_=tf;
      componentRelations.push_back(compRelation);
      std::cout<<"Initialized TrackFitter "<<name_s<<std::endl;
    } 
  
  }
  
  // Read ROOT event files and iterate
  
  TFile *eventFile=new TFile((inputDir+"/"+inputFile).c_str());
  TTree *tree=(TTree*)eventFile->Get("ntupler/tree");
  
  std::vector<float> *stubs_modId=0;
  std::vector<float> *stubs_r=0;
  std::vector<std::vector<std::vector<unsigned int> > > *roads_stubRefs=0;
  std::vector<float> *tracks_eta=0;
  
  tree->SetBranchAddress("TTStubs_modId", &(stubs_modId));
  tree->SetBranchAddress("TTStubs_r", &(stubs_r));
  tree->SetBranchAddress("AMTTRoads_stubRefs", &(roads_stubRefs));
  tree->SetBranchAddress("AMTTTracks_eta", &(tracks_eta));
  
  // Book histograms for the FIFO depth
  TH1F *h_AM_FIFO_0=new TH1F("h_AM_FIFO_0", "Depth of AM FIFO 0", 500, 0, 300);
  TH1F *h_AM_FIFO_1=new TH1F("h_AM_FIFO_1", "Depth of AM FIFO 1", 500, 0, 300);
  TH1F *h_AM_FIFO_2=new TH1F("h_AM_FIFO_2", "Depth of AM FIFO 2", 500, 0, 300);
  TH1F *h_AM_FIFO_3=new TH1F("h_AM_FIFO_3", "Depth of AM FIFO 3", 500, 0, 300);
  TH1F *h_AM_FIFO_4=new TH1F("h_AM_FIFO_4", "Depth of AM FIFO 4", 500, 0, 300);
  TH1F *h_AM_FIFO_5=new TH1F("h_AM_FIFO_5", "Depth of AM FIFO 5", 500, 0, 300);
  DataSource *ds1=(DataSource*)componentRelations.at(0)->comp_;
  AssociativeMemory *am1=(AssociativeMemory*)componentRelations.at(2)->comp_;
  double amRatio=1.-(ds1->getOutTime()/am1->getInTime());
  if (amRatio<0 || amRatio>1) amRatio=0;
  
  TH1F *h_CB_FIFO=new TH1F("h_CB_FIFO", "Depth of CB FIFO", 500, 0, 300);
  HitBuffer *hb1=(HitBuffer*)componentRelations.at(3)->comp_;
  CombinationBuilder *cb1=(CombinationBuilder*)componentRelations.at(4)->comp_;
  double cbRatio=1.-(hb1->getOutTime()/cb1->getInTime());
  if (cbRatio<0 || cbRatio>1) cbRatio=0;
  
  TH1F *h_TF_FIFO=new TH1F("h_TF_FIFO", "Depth of TF FIFO", 500, 0, 300);
  TrackFitter *tf1=(TrackFitter*)componentRelations.at(5)->comp_;
  double tfRatio=1.-(cb1->getOutTime()/tf1->getInTime());
  if (tfRatio<0 || tfRatio>1) tfRatio=0;
  
  unsigned int nEvents=tree->GetEntries();
  for (unsigned int i_event=0; i_event<nEvents; ++i_event)
  {
    tree->GetEntry(i_event);
    EventCharacteristics event(stubs_modId, stubs_r, roads_stubRefs, tracks_eta);
    
    /*
    EventCharacteristics event;
    for (unsigned int i=0; i<6; ++i)
    {
      event.nStubs_layer.at(i)=0;
    }
    event.nPatterns=0;
    event.nOutwords=0;
    event.nCombinations=0;
    event.nTracks=0;
    
    std::cout<<"=== Event === "<<std::endl;
    for (unsigned int i=0; i<6; ++i)
    {
      std::cout<<"event.nStubs_layer.at("<<i<<") = "<<event.nStubs_layer.at(i)<<std::endl;
    }
    std::cout<<"event.nPatterns = "<<event.nPatterns<<std::endl;
    std::cout<<"event.nOutwords = "<<event.nOutwords<<std::endl;
    std::cout<<"event.nCombinations = "<<event.nCombinations<<std::endl;
    std::cout<<"event.nTracks = "<<event.nTracks<<std::endl;
    std::cout<<"=== ==="<<std::endl;
    */
    
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
    
    // Calculate length of FIFOs
    h_AM_FIFO_0->Fill(amRatio*double(event.nStubs_layer.at(0)));
    h_AM_FIFO_1->Fill(amRatio*double(event.nStubs_layer.at(1)));
    h_AM_FIFO_2->Fill(amRatio*double(event.nStubs_layer.at(2)));
    h_AM_FIFO_3->Fill(amRatio*double(event.nStubs_layer.at(3)));
    h_AM_FIFO_4->Fill(amRatio*double(event.nStubs_layer.at(4)));
    h_AM_FIFO_5->Fill(amRatio*double(event.nStubs_layer.at(5)));
    h_CB_FIFO->Fill(cbRatio*double(event.nOutwords));
    h_TF_FIFO->Fill(tfRatio*double(event.nCombinations));
    
    if (i_event%1000==0) std::cout<<"Events "<<i_event<<" out of "<<nEvents<<" have been processed."<<std::endl;
  }
  
  TFile *f_FIFO=new TFile("fifos.root", "recreate");
  h_AM_FIFO_0->Write();
  h_AM_FIFO_1->Write();
  h_AM_FIFO_2->Write();
  h_AM_FIFO_3->Write();
  h_AM_FIFO_4->Write();
  h_AM_FIFO_5->Write();
  h_CB_FIFO->Write();
  h_TF_FIFO->Write();
  f_FIFO->Write();
  
  // Write component histograms to file
  for (unsigned int i_comp=0; i_comp<componentRelations.size(); ++i_comp)
  {
    Component *component=componentRelations.at(i_comp)->comp_;
    component->writeHistograms();
  }
  
  return 0;
  
}
