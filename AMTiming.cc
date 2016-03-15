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

#include "src/CommandLineArguments.cc"
#include "src/ReadConfigurationLine.cc"

bool findClass(Component *component, std::string name)
{
  std::string classname=std::string(typeid(*component).name());
  if (classname.find(name)!=std::string::npos) return true;
  else return false;
}

std::string argInLine(std::string line, std::string key)
{
  removeSpaces(line);
  std::size_t pos=line.find(key);
  if (pos==std::string::npos)
  {
    return "";
  }
  else
  {
    return line.substr(key.size()+1);
  }
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
  int index=-1;
  while (!file.eof())
  {
    getline(file, s);
    removeSpaces(s);
    
    if (s=="") continue; // This is an empty line
    if (s.at(0)=='#') continue; // This is a comment
    
    if (s.find("Index")!=std::string::npos)
    {
      int newIndex=atoi(argInLine(s, "Index").c_str());
      if (newIndex==index+1)
      {
        index=newIndex;
        getline(file, s);
        std::string componentType=argInLine(s, "ComponentType");
        
        if (componentType=="DataSource")
        {
          std::map<std::string, std::string> map_DataSource=readConfigurationLine(&file, s);
          std::string name_s; if (map_DataSource.find("ComponentName")!=map_DataSource.end()) name_s=map_DataSource["ComponentName"]; else std::cout<<"ERROR: DataSource ComponentName does not exist in configuration file"<<std::endl;
          double frequency;   if (map_DataSource.find("Frequency")!=map_DataSource.end()) frequency=atof(map_DataSource["Frequency"].c_str()); else std::cout<<"ERROR: DataSource Frequency does not exist in configuration file"<<std::endl;
          double outClock;    if (map_DataSource.find("OutClock")!=map_DataSource.end()) outClock=atof(map_DataSource["OutClock"].c_str()); else std::cout<<"ERROR: DataSource OutClock does not exist in configuration file"<<std::endl;
          double outTime=outClock/frequency*1000.;
          DataSource *ds=new DataSource(name_s, outTime);
          ComponentRelation *compRelation=new ComponentRelation();
          while (s!="")
          {
            std::string outputConnections=argInLine(s, "OutputConnections");
            compRelation->extractComponentRelation(outputConnections);
            getline(file, s);
          }
          compRelation->comp_=ds;
          componentRelations.push_back(compRelation);
        }
        
        if (componentType=="StubMapper")
        {
          std::map<std::string, std::string> map_StubMapper=readConfigurationLine(&file, s);
          std::string name_s; if (map_StubMapper.find("ComponentName")!=map_StubMapper.end()) name_s=map_StubMapper["ComponentName"]; else std::cout<<"ERROR: StubMapper ComponentName does not exist in configuration file"<<std::endl;
          double frequency;   if (map_StubMapper.find("Frequency")!=map_StubMapper.end()) frequency=atof(map_StubMapper["Frequency"].c_str()); else std::cout<<"ERROR: StubMapper Frequency does not exist in configuration file"<<std::endl;
          double delayClock;  if (map_StubMapper.find("DelayClock")!=map_StubMapper.end()) delayClock=atof(map_StubMapper["DelayClock"].c_str()); else std::cout<<"ERROR: DataSource DelayClock does not exist in configuration file"<<std::endl;
          double delay=delayClock/frequency*1000.;
          StubMapper *sm=new StubMapper(name_s, delay);
          ComponentRelation *compRelation=new ComponentRelation();
          while (s!="")
          {
            std::string outputConnections=argInLine(s, "OutputConnections");
            compRelation->extractComponentRelation(outputConnections);
            getline(file, s);
          }
          compRelation->comp_=sm;
          componentRelations.push_back(compRelation);
        }
        
        if (componentType=="AssociativeMemory")
        {
          std::map<std::string, std::string> map_AssociativeMemory=readConfigurationLine(&file, s);
          std::string name_s; if (map_AssociativeMemory.find("ComponentName")!=map_AssociativeMemory.end()) name_s=map_AssociativeMemory["ComponentName"]; else std::cout<<"ERROR: AssociateMemory ComponentName does not exist in configuration file"<<std::endl;
          double frequency;   if (map_AssociativeMemory.find("Frequency")!=map_AssociativeMemory.end()) frequency=atof(map_AssociativeMemory["Frequency"].c_str()); else std::cout<<"ERROR: AssociativeMemory Frequency does not exist in configuration file"<<std::endl;
          double inClock;     if (map_AssociativeMemory.find("InClock")!=map_AssociativeMemory.end()) inClock=atof(map_AssociativeMemory["InClock"].c_str()); else std::cout<<"ERROR: AssociativeMemory InClock does not exist in configuration file"<<std::endl;
          double delayClock;  if (map_AssociativeMemory.find("DelayClock")!=map_AssociativeMemory.end()) delayClock=atof(map_AssociativeMemory["DelayClock"].c_str()); else std::cout<<"ERROR: AssociativeMemory DelayClock does not exist in configuration file"<<std::endl;
          double outClock;    if (map_AssociativeMemory.find("OutClock")!=map_AssociativeMemory.end()) outClock=atof(map_AssociativeMemory["OutClock"].c_str()); else std::cout<<"ERROR: AssociativeMemory OutClock does not exist in configuration file"<<std::endl;
          double inTime=inClock/frequency*1000.;
          double delay=delayClock/frequency*1000.;
          double outTime=outClock/frequency*1000.;
          AssociativeMemory *am=new AssociativeMemory(name_s, delay, inTime, outTime);
          ComponentRelation *compRelation=new ComponentRelation();
          while (s!="")
          {
            std::string outputConnections=argInLine(s, "OutputConnections");
            compRelation->extractComponentRelation(outputConnections);
            getline(file, s);
          }
          compRelation->comp_=am;
          componentRelations.push_back(compRelation);
        }
        
        if (componentType=="HitBuffer")
        {
          std::map<std::string, std::string> map_HitBuffer=readConfigurationLine(&file, s);
          std::string name_s; if (map_HitBuffer.find("ComponentName")!=map_HitBuffer.end()) name_s=map_HitBuffer["ComponentName"]; else std::cout<<"ERROR: HitBuffer ComponentName does not exist in configuration file"<<std::endl;
          double frequency;   if (map_HitBuffer.find("Frequency")!=map_HitBuffer.end()) frequency=atof(map_HitBuffer["Frequency"].c_str()); else std::cout<<"ERROR: HitBuffer Frequency does not exist in configuration file"<<std::endl;
          double inClock;     if (map_HitBuffer.find("InClock")!=map_HitBuffer.end()) inClock=atof(map_HitBuffer["InClock"].c_str()); else std::cout<<"ERROR: HitBuffer InClock does not exist in configuration file"<<std::endl;
          double procClock;   if (map_HitBuffer.find("ProcClock")!=map_HitBuffer.end()) procClock=atof(map_HitBuffer["ProcClock"].c_str()); else std::cout<<"ERROR: HitBuffer ProcClock does not exist in configuration file"<<std::endl;
          double delayClock;  if (map_HitBuffer.find("DelayClock")!=map_HitBuffer.end()) delayClock=atof(map_HitBuffer["DelayClock"].c_str()); else std::cout<<"ERROR: HitBuffer DelayClock does not exist in configuration file"<<std::endl;
          double outClock;    if (map_HitBuffer.find("OutClock")!=map_HitBuffer.end()) outClock=atof(map_HitBuffer["OutClock"].c_str()); else std::cout<<"ERROR: HitBuffer OutClock does not exist in configuration file"<<std::endl;
          double inTime=inClock/frequency*1000.;
          double procTime=procClock/frequency*1000.;
          double delay=delayClock/frequency*1000.;
          double outTime=outClock/frequency*1000.;
          HitBuffer *hb=new HitBuffer(name_s, inTime, procTime, outTime, delay);
          ComponentRelation *compRelation=new ComponentRelation();
          while (s!="")
          {
            std::string outputConnections=argInLine(s, "OutputConnections");
            compRelation->extractComponentRelation(outputConnections);
            getline(file, s);
          }
          compRelation->comp_=hb;
          componentRelations.push_back(compRelation);
        }
        
        if (componentType=="CombinationBuilder")
        {
          std::map<std::string, std::string> map_CombinationBuilder=readConfigurationLine(&file, s);
          std::string name_s; if (map_CombinationBuilder.find("ComponentName")!=map_CombinationBuilder.end()) name_s=map_CombinationBuilder["ComponentName"]; else std::cout<<"ERROR: CombinationBuilder ComponentName does not exist in configuration file"<<std::endl;
          double frequency;   if (map_CombinationBuilder.find("Frequency")!=map_CombinationBuilder.end()) frequency=atof(map_CombinationBuilder["Frequency"].c_str()); else std::cout<<"ERROR: CombinationBuilder Frequency does not exist in configuration file"<<std::endl;
          double inClock;     if (map_CombinationBuilder.find("InClock")!=map_CombinationBuilder.end()) inClock=atof(map_CombinationBuilder["InClock"].c_str()); else std::cout<<"ERROR: CombinationBuilder InClock does not exist in configuration file"<<std::endl;
          double delayClock;  if (map_CombinationBuilder.find("DelayClock")!=map_CombinationBuilder.end()) delayClock=atof(map_CombinationBuilder["DelayClock"].c_str()); else std::cout<<"ERROR: CombinationBuilder DelayClock does not exist in configuration file"<<std::endl;
          double outClock;    if (map_CombinationBuilder.find("OutClock")!=map_CombinationBuilder.end()) outClock=atof(map_CombinationBuilder["OutClock"].c_str()); else std::cout<<"ERROR: CombinationBuilder OutClock does not exist in configuration file"<<std::endl;
          double inTime=inClock/frequency*1000.;
          double delay=delayClock/frequency*1000.;
          double outTime=outClock/frequency*1000.;
          CombinationBuilder *cb=new CombinationBuilder(name_s, inTime, outTime, delay);
          ComponentRelation *compRelation=new ComponentRelation();
          while (s!="")
          {
            std::string outputConnections=argInLine(s, "OutputConnections");
            compRelation->extractComponentRelation(outputConnections);
            getline(file, s);
          }
          compRelation->comp_=cb;
          componentRelations.push_back(compRelation);
        }
        
        if (componentType=="TrackFitter")
        {
          std::map<std::string, std::string> map_TrackFitter=readConfigurationLine(&file, s);
          std::string name_s; if (map_TrackFitter.find("ComponentName")!=map_TrackFitter.end()) name_s=map_TrackFitter["ComponentName"]; else std::cout<<"ERROR: TrackFitter ComponentName does not exist in configuration file"<<std::endl;
          double frequency;   if (map_TrackFitter.find("Frequency")!=map_TrackFitter.end()) frequency=atof(map_TrackFitter["Frequency"].c_str()); else std::cout<<"ERROR: TrackFitter Frequency does not exist in configuration file"<<std::endl;
          double inClock;     if (map_TrackFitter.find("InClock")!=map_TrackFitter.end()) inClock=atof(map_TrackFitter["InClock"].c_str()); else std::cout<<"ERROR: TrackFitter InClock does not exist in configuration file"<<std::endl;
          double delayClock;  if (map_TrackFitter.find("DelayClock")!=map_TrackFitter.end()) delayClock=atof(map_TrackFitter["DelayClock"].c_str()); else std::cout<<"ERROR: TrackFitter DelayClock does not exist in configuration file"<<std::endl;
          double outClock;    if (map_TrackFitter.find("OutClock")!=map_TrackFitter.end()) outClock=atof(map_TrackFitter["OutClock"].c_str()); else std::cout<<"ERROR: TrackFitter OutClock does not exist in configuration file"<<std::endl;
          double inTime=inClock/frequency*1000.;
          double delay=delayClock/frequency*1000.;
          double outTime=outClock/frequency*1000.;
          TrackFitter *tf=new TrackFitter(name_s, inTime, outTime, delay);
          ComponentRelation *compRelation=new ComponentRelation();
          while (s!="")
          {
            std::string outputConnections=argInLine(s, "OutputConnections");
            compRelation->extractComponentRelation(outputConnections);
            getline(file, s);
          }
          compRelation->comp_=tf;
          componentRelations.push_back(compRelation);
        }
        
      } // Check for new index
      else
      {
        std::cout<<"WARNING: Index of this component "<<newIndex<<" does not follow index of previous component "<<index<<"."<<std::endl;
      }
    } // Found new Index
  }
  
  // Read ROOT event files and iterate
  TFile *eventFile=new TFile((inputDir+"/"+inputFile).c_str());
  TTree *tree=(TTree*)eventFile->Get("ntupler/tree");
  
  std::vector<float> *stubs_modId=0;
  std::vector<float> *stubs_r=0;
  std::vector<std::vector<std::vector<unsigned int> > > *roads_stubRefs=0;
  std::vector<unsigned int> *tracks_roadRef=0;
  
  tree->SetBranchAddress("TTStubs_modId", &(stubs_modId));
  tree->SetBranchAddress("TTStubs_r", &(stubs_r));
  tree->SetBranchAddress("AMTTRoads_stubRefs", &(roads_stubRefs));
  tree->SetBranchAddress("AMTTTracks_roadRef", &(tracks_roadRef));
  
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
    
    EventCharacteristics eventCharacteristics(stubs_modId, stubs_r, roads_stubRefs, tracks_roadRef);
    
    // Quick test to only check single muon events with 6 stubs
    // to establish contact with hardware testing
    /*if (!(eventCharacteristics.nStubs_layer.at(0)==1 &&
          eventCharacteristics.nStubs_layer.at(1)==1 &&
          eventCharacteristics.nStubs_layer.at(2)==1 &&
          eventCharacteristics.nStubs_layer.at(3)==1 &&
          eventCharacteristics.nStubs_layer.at(4)==1 &&
          eventCharacteristics.nStubs_layer.at(5)==1)) continue;*/
    
    // iterate over componentRelations
    for (unsigned int i_comp=0; i_comp<componentRelations.size(); ++i_comp)
    {
      ComponentRelation *componentRelation=componentRelations.at(i_comp);
      Component *component=componentRelation->comp_;
      if (component->get_type()=="DataSource") component->setEventCharacteristics(eventCharacteristics);
      component->computeOutputTimes();
      
      // Printout outputs of this component
      // component->printOutputTimes();
      
      // How many outputs does this component have?
      // Connect all of them to specified inputs of the i_comp
      // Also count number of connected components for eventCharacteristics splitting
      std::vector<StubMapper*> stubMappers;
      std::vector<AssociativeMemory*> associativeMemories;
      std::vector<HitBuffer*> hitBuffers;
      std::vector<CombinationBuilder*> combinationBuilders;
      std::vector<TrackFitter*> trackFitters;
      std::string targetSMName="";
      std::string targetAMName="";
      std::string targetHBName="";
      std::string targetCBName="";
      std::string targetTFName="";
      for (unsigned int i_pin=0; i_pin<componentRelation->i_comp_.size(); ++i_pin)
      {
        if (componentRelation->i_comp_.at(i_pin)<componentRelations.size())
        {
          Component *targetComponent=componentRelations.at(componentRelation->i_comp_.at(i_pin))->comp_;
          targetComponent->set_t1in(componentRelation->i_input_.at(i_pin), component->get_t1out(componentRelation->i_output_.at(i_pin)));
          targetComponent->set_t2in(componentRelation->i_input_.at(i_pin), component->get_t2out(componentRelation->i_output_.at(i_pin)));
          
          if (targetComponent->get_type()=="StubMapper" && targetComponent->get_name()!=targetSMName)
          {
            stubMappers.push_back((StubMapper*)targetComponent);
            targetSMName=targetComponent->get_name();
          }
          else if (targetComponent->get_type()=="AssociativeMemory" && targetComponent->get_name()!=targetAMName) 
          {
            associativeMemories.push_back((AssociativeMemory*)targetComponent);
            targetAMName=targetComponent->get_name();
          }
          else if (targetComponent->get_type()=="HitBuffer" && targetComponent->get_name()!=targetHBName)
          {
            hitBuffers.push_back((HitBuffer*)targetComponent);
            targetHBName=targetComponent->get_name();
          }
          else if (targetComponent->get_type()=="CombinationBuilder" && targetComponent->get_name()!=targetCBName)
          {
            combinationBuilders.push_back((CombinationBuilder*)targetComponent);
            targetCBName=targetComponent->get_name();
          }
          else if (targetComponent->get_type()=="TrackFitter" && targetComponent->get_name()!=targetTFName)
          {
            trackFitters.push_back((TrackFitter*)targetComponent);
            targetTFName=targetComponent->get_name();
          } 
        }
        else
        {
          std::cout<<"ERROR: Schematic file. Output pins of component "<<component->get_name()<<" refer to non-existent indices in schematic."<<std::endl;
        }
      }
      
      if (component->get_type()=="DataSource")
      {
        if (stubMappers.size()==0) std::cout<<"ERROR: DataSource "<<component->get_name()<<" is not connected to a StubMapper."<<std::endl;
        for (unsigned int i_sm=0; i_sm<stubMappers.size(); ++i_sm)
        {
          stubMappers.at(i_sm)->setEventCharacteristics(*(component->getEventCharacteristics()));
        }
      }
      else if (component->get_type()=="StubMapper")
      {
        if (associativeMemories.size()!=0)
        {
          if (hitBuffers.size()!=associativeMemories.size()) std::cout<<"ERROR: Number of HitBuffers connected to StubMapper "<<component->get_name()<<" is not equal to the number of AssociativeMemories."<<std::endl;
          std::vector<EventCharacteristics> events;
          component->getEventCharacteristics()->splitEventAtAM(associativeMemories.size(), &events);
          for (unsigned int i_am=0; i_am<associativeMemories.size(); ++i_am)
          {
            associativeMemories.at(i_am)->setEventCharacteristics(events.at(i_am));
            hitBuffers.at(i_am)->setEventCharacteristics(events.at(i_am));
          }
        }
        else if (combinationBuilders.size()!=0)
        {
          for (unsigned int i_cb=0; i_cb<combinationBuilders.size(); ++i_cb)
          {
            combinationBuilders.at(i_cb)->setEventCharacteristics(*(component->getEventCharacteristics()));
          }
        }
        else std::cout<<"ERROR: StubMapper "<<component->get_name()<<" is not connected to an AssociativeMemory or a CombinationBuilder."<<std::endl;
          
      }
      else if (component->get_type()=="HitBuffer")
      {
        if (combinationBuilders.size()!=0)
        {
          for (unsigned int i_cb=0; i_cb<combinationBuilders.size(); ++i_cb)
          {
            combinationBuilders.at(i_cb)->setEventCharacteristics(*(component->getEventCharacteristics()));
          }
        }
        else if (stubMappers.size()!=0)
        {
          for (unsigned int i_sm=0; i_sm<stubMappers.size(); ++i_sm)
          {
            stubMappers.at(i_sm)->setEventCharacteristics(*(component->getEventCharacteristics()));
          }
        }
        else std::cout<<"ERROR: HitBuffer "<<component->get_name()<<" is not connected to a CombinationBuilder or a StubMapper."<<std::endl;
      }
      else if (component->get_type()=="CombinationBuilder")
      {
        if (trackFitters.size()==0) std::cout<<"ERROR: CombinationBuilder "<<component->get_name()<<" is connected to a TrackFitter."<<std::endl;
        for (unsigned int i_tf=0; i_tf<trackFitters.size(); ++i_tf)
        {
          trackFitters.at(i_tf)->setEventCharacteristics(*(component->getEventCharacteristics()));
        }
      } 
      
    }
   
    // Calculate length of FIFOs
    // h_AM_FIFO_0->Fill(amRatio*double(event->nStubs_layer.at(0)));
    // h_AM_FIFO_1->Fill(amRatio*double(event->nStubs_layer.at(1)));
    // h_AM_FIFO_2->Fill(amRatio*double(event->nStubs_layer.at(2)));
    // h_AM_FIFO_3->Fill(amRatio*double(event.nStubs_layer.at(3)));
    // h_AM_FIFO_4->Fill(amRatio*double(event.nStubs_layer.at(4)));
    // h_AM_FIFO_5->Fill(amRatio*double(event.nStubs_layer.at(5)));
    // h_CB_FIFO->Fill(cbRatio*double(event.nOutwords));
    // h_TF_FIFO->Fill(tfRatio*double(event.nCombinations));
  
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
