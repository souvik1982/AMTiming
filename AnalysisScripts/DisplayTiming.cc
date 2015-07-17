#include <TH1F.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TFile.h>

#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

#include "BlockDiagram.cc"

bool logScale=true;

void removeSpaces(std::string &input)
{
  input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
}

std::string ftoa2(double i) 
{
  char res[10];
  sprintf(res, "%2.2f", i);
  std::string ret(res);
  return ret;
}

int fixRange(TH1F *h)
{
  double nBinsHave=h->FindLastBinAbove(0, 1)+1;
  double nEntries=h->GetEntries();
  int rebin=0;  
  if (nEntries>0 && nBinsHave>0)
  {
    rebin=int(200.*nBinsHave/nEntries);
    if (rebin>0) h->Rebin(rebin);
    else rebin=-1;
  }
  h->GetXaxis()->SetRange(0, h->FindLastBinAbove(0, 1)+1);
  return rebin;
}

void makeCanvas(TH1F *h1, std::string name_s, std::string componentType, std::string units="", int color=kBlue)
{
  h1->SetLineColor(color);
  fixRange(h1);
  
  TCanvas *c=new TCanvas(("c_"+name_s).c_str(), ("c_"+name_s).c_str(), 700, 700);
  if (logScale) c->SetLogy();
  h1->SetTitle(("; "+componentType+" "+name_s+" ("+units+")").c_str());
  h1->Draw("");
  TLegend *leg=new TLegend(0.6, 0.7, 0.89, 0.89);
  leg->SetBorderSize(0);
  leg->AddEntry(h1, ("("+ftoa2(h1->GetMean())+", "+ftoa2(h1->GetRMS())+") "+units).c_str());
  leg->Draw();
  c->SaveAs(("c_"+name_s+".png").c_str());
}

void makeCanvas(TH1F *h1, TH1F *h2, std::string name_s, std::string componentType, std::string units="")
{
  h2->SetLineColor(kGreen+2);
  int rebin=fixRange(h2);
  if (rebin>0) h1->Rebin(rebin);
  TCanvas *c=new TCanvas(("c_"+name_s).c_str(), ("c_"+name_s).c_str(), 700, 700);
  if (logScale) c->SetLogy();
  h2->SetTitle(("; "+componentType+" "+name_s+" ("+units+")").c_str());
  h2->SetMaximum(h1->GetMaximum()*1.2);
  h2->Draw("");
  h1->Draw("same");
  h2->Draw("same");
  TLegend *leg=new TLegend(0.6, 0.7, 0.89, 0.89);
  leg->SetBorderSize(0);
  leg->AddEntry(h1, ("t1out ("+ftoa2(h1->GetMean())+", "+ftoa2(h1->GetRMS())+") "+units).c_str());
  leg->AddEntry(h2, ("t2out ("+ftoa2(h2->GetMean())+", "+ftoa2(h2->GetRMS())+") "+units).c_str());
  leg->Draw();
  
  c->SaveAs(("c_"+name_s+".png").c_str());
}

void DisplayTiming(std::string title)
{
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  
  BlockDiagram blockDiagram_0p50(0.5, title);
  BlockDiagram blockDiagram_0p999(0.999, title);
  
  ofstream outfile;
  outfile.open("AMTimingAnalysis.html");
  outfile<<"<html>"<<std::endl;
  outfile<<"<head>"<<std::endl;
  outfile<<"</head>"<<std::endl;
  outfile<<"<body>"<<std::endl;
  outfile<<"<h1 align='center'> AM Mezzanine Board Timing Dashboard </h1>"<<std::endl;
  outfile<<"<table border='1'>"<<std::endl;
  
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
      removeSpaces(name_s);
      
      TFile *ds=new TFile((""+name_s+".root").c_str());
      TH1F *h_t2out_ds_0=(TH1F*)ds->Get(("h_t2out_"+name_s+"_0").c_str());
      TH1F *h_t2out_ds_1=(TH1F*)ds->Get(("h_t2out_"+name_s+"_1").c_str());
      TH1F *h_t2out_ds_2=(TH1F*)ds->Get(("h_t2out_"+name_s+"_2").c_str());
      TH1F *h_t2out_ds_3=(TH1F*)ds->Get(("h_t2out_"+name_s+"_3").c_str());
      TH1F *h_t2out_ds_4=(TH1F*)ds->Get(("h_t2out_"+name_s+"_4").c_str());
      TH1F *h_t2out_ds_5=(TH1F*)ds->Get(("h_t2out_"+name_s+"_5").c_str());
      TH1F *h_nStubs_ds_0=(TH1F*)ds->Get(("h_nStubs_"+name_s+"_0").c_str());
      TH1F *h_nStubs_ds_1=(TH1F*)ds->Get(("h_nStubs_"+name_s+"_1").c_str());
      TH1F *h_nStubs_ds_2=(TH1F*)ds->Get(("h_nStubs_"+name_s+"_2").c_str());
      TH1F *h_nStubs_ds_3=(TH1F*)ds->Get(("h_nStubs_"+name_s+"_3").c_str());
      TH1F *h_nStubs_ds_4=(TH1F*)ds->Get(("h_nStubs_"+name_s+"_4").c_str());
      TH1F *h_nStubs_ds_5=(TH1F*)ds->Get(("h_nStubs_"+name_s+"_5").c_str());
      
      makeCanvas(h_t2out_ds_0, name_s+"_0", "Data Source", "ns");
      makeCanvas(h_t2out_ds_1, name_s+"_1", "Data Source", "ns");
      makeCanvas(h_t2out_ds_2, name_s+"_2", "Data Source", "ns");
      makeCanvas(h_t2out_ds_3, name_s+"_3", "Data Source", "ns");
      makeCanvas(h_t2out_ds_4, name_s+"_4", "Data Source", "ns");
      makeCanvas(h_t2out_ds_5, name_s+"_5", "Data Source", "ns");
      
      makeCanvas(h_nStubs_ds_0, name_s+"_nStubs_0", "Stub Occupancy", "", kRed);
      makeCanvas(h_nStubs_ds_1, name_s+"_nStubs_1", "Stub Occupancy", "", kRed);
      makeCanvas(h_nStubs_ds_2, name_s+"_nStubs_2", "Stub Occupancy", "", kRed);
      makeCanvas(h_nStubs_ds_3, name_s+"_nStubs_3", "Stub Occupancy", "", kRed);
      makeCanvas(h_nStubs_ds_4, name_s+"_nStubs_4", "Stub Occupancy", "", kRed);
      makeCanvas(h_nStubs_ds_5, name_s+"_nStubs_5", "Stub Occupancy", "", kRed);
      
      outfile<<"  <tr>"<<std::endl;
      outfile<<"    <td align='center'>"<<std::endl;
      outfile<<"      <h2> DataSource "<<name_s<<"</h2> "<<std::endl;
      outfile<<"      outTime = "<<outTime_s<<" ns <br/> "<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"    <td>"<<std::endl;
      outfile<<"      <table border='1'>"<<std::endl;
      outfile<<"        <tr>"<<std::endl;
      outfile<<"          <td align='center'>"<<std::endl;
      outfile<<"            <img src='"<<("c_"+name_s+"_nStubs_0.png")<<"'/"<<std::endl;
      outfile<<"          </td>"<<std::endl;
      outfile<<"          <td align='center'>"<<std::endl;
      outfile<<"            <img src='"<<("c_"+name_s+"_nStubs_1.png")<<"'/"<<std::endl;
      outfile<<"          </td>"<<std::endl;
      outfile<<"          <td align='center'>"<<std::endl;
      outfile<<"            <img src='"<<("c_"+name_s+"_nStubs_2.png")<<"'/"<<std::endl;
      outfile<<"          </td>"<<std::endl;
      outfile<<"          <td align='center'>"<<std::endl;
      outfile<<"            <img src='"<<("c_"+name_s+"_nStubs_3.png")<<"'/"<<std::endl;
      outfile<<"          </td>"<<std::endl;
      outfile<<"          <td align='center'>"<<std::endl;
      outfile<<"            <img src='"<<("c_"+name_s+"_nStubs_4.png")<<"'/"<<std::endl;
      outfile<<"          </td>"<<std::endl;
      outfile<<"          <td align='center'>"<<std::endl;
      outfile<<"            <img src='"<<("c_"+name_s+"_nStubs_5.png")<<"'/"<<std::endl;
      outfile<<"          </td>"<<std::endl;
      outfile<<"        </tr>"<<std::endl;
      outfile<<"        <tr>"<<std::endl;
      outfile<<"          <td>"<<std::endl;
      outfile<<"            <img src='"<<("c_"+name_s+"_0.png")<<"'/>"<<std::endl;
      outfile<<"          </td>"<<std::endl;
      outfile<<"          <td>"<<std::endl;
      outfile<<"            <img src='"<<("c_"+name_s+"_1.png")<<"'/>"<<std::endl;
      outfile<<"          </td>"<<std::endl;
      outfile<<"          <td>"<<std::endl;
      outfile<<"            <img src='"<<("c_"+name_s+"_2.png")<<"'/>"<<std::endl;
      outfile<<"          </td>"<<std::endl;
      outfile<<"          <td>"<<std::endl;
      outfile<<"            <img src='"<<("c_"+name_s+"_3.png")<<"'/>"<<std::endl;
      outfile<<"          </td>"<<std::endl;
      outfile<<"          <td>"<<std::endl;
      outfile<<"            <img src='"<<("c_"+name_s+"_4.png")<<"'/>"<<std::endl;
      outfile<<"          </td>"<<std::endl;
      outfile<<"          <td>"<<std::endl;
      outfile<<"            <img src='"<<("c_"+name_s+"_5.png")<<"'/>"<<std::endl;
      outfile<<"          </td>"<<std::endl;
      outfile<<"        </tr>"<<std::endl;
      outfile<<"      </table>"<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"  </tr>"<<std::endl;
      
      blockDiagram_0p50.addComponent("DataSource_"+name_s+"_0", kAzure+6, 0, h_t2out_ds_0);
      blockDiagram_0p50.addComponent("DataSource_"+name_s+"_1", kAzure+6, 0, h_t2out_ds_1);
      blockDiagram_0p50.addComponent("DataSource_"+name_s+"_2", kAzure+6, 0, h_t2out_ds_2);
      blockDiagram_0p50.addComponent("DataSource_"+name_s+"_3", kAzure+6, 0, h_t2out_ds_3);
      blockDiagram_0p50.addComponent("DataSource_"+name_s+"_4", kAzure+6, 0, h_t2out_ds_4);
      blockDiagram_0p50.addComponent("DataSource_"+name_s+"_5", kAzure+6, 0, h_t2out_ds_5);
      
      blockDiagram_0p999.addComponent("DataSource_"+name_s+"_0", kAzure+6, 0, h_t2out_ds_0);
      blockDiagram_0p999.addComponent("DataSource_"+name_s+"_1", kAzure+6, 0, h_t2out_ds_1);
      blockDiagram_0p999.addComponent("DataSource_"+name_s+"_2", kAzure+6, 0, h_t2out_ds_2);
      blockDiagram_0p999.addComponent("DataSource_"+name_s+"_3", kAzure+6, 0, h_t2out_ds_3);
      blockDiagram_0p999.addComponent("DataSource_"+name_s+"_4", kAzure+6, 0, h_t2out_ds_4);
      blockDiagram_0p999.addComponent("DataSource_"+name_s+"_5", kAzure+6, 0, h_t2out_ds_5);
    }
    
    if (component_s.find("StubMapper")!=std::string::npos)
    {
      std::string name_s, delay_s;
      getline(ss, name_s, '|');
      getline(ss, delay_s, '|');
      removeSpaces(name_s);
      
      TFile *sm=new TFile((""+name_s+".root").c_str());
      TH1F *h_t1out_sm_0=(TH1F*)sm->Get(("h_t1out_"+name_s+"_0").c_str());
      TH1F *h_t1out_sm_1=(TH1F*)sm->Get(("h_t1out_"+name_s+"_1").c_str());
      TH1F *h_t1out_sm_2=(TH1F*)sm->Get(("h_t1out_"+name_s+"_2").c_str());
      TH1F *h_t1out_sm_3=(TH1F*)sm->Get(("h_t1out_"+name_s+"_3").c_str());
      TH1F *h_t1out_sm_4=(TH1F*)sm->Get(("h_t1out_"+name_s+"_4").c_str());
      TH1F *h_t1out_sm_5=(TH1F*)sm->Get(("h_t1out_"+name_s+"_5").c_str());
      TH1F *h_t2out_sm_0=(TH1F*)sm->Get(("h_t2out_"+name_s+"_0").c_str());
      TH1F *h_t2out_sm_1=(TH1F*)sm->Get(("h_t2out_"+name_s+"_1").c_str());
      TH1F *h_t2out_sm_2=(TH1F*)sm->Get(("h_t2out_"+name_s+"_2").c_str());
      TH1F *h_t2out_sm_3=(TH1F*)sm->Get(("h_t2out_"+name_s+"_3").c_str());
      TH1F *h_t2out_sm_4=(TH1F*)sm->Get(("h_t2out_"+name_s+"_4").c_str());
      TH1F *h_t2out_sm_5=(TH1F*)sm->Get(("h_t2out_"+name_s+"_5").c_str());
      
      makeCanvas(h_t1out_sm_0, h_t2out_sm_0, name_s+"_0", "StubMapper", "ns");
      makeCanvas(h_t1out_sm_1, h_t2out_sm_1, name_s+"_1", "StubMapper", "ns");
      makeCanvas(h_t1out_sm_2, h_t2out_sm_2, name_s+"_2", "StubMapper", "ns");
      makeCanvas(h_t1out_sm_3, h_t2out_sm_3, name_s+"_3", "StubMapper", "ns");
      makeCanvas(h_t1out_sm_4, h_t2out_sm_4, name_s+"_4", "StubMapper", "ns");
      makeCanvas(h_t1out_sm_5, h_t2out_sm_5, name_s+"_5", "StubMapper", "ns");
      
      outfile<<"  <tr>"<<std::endl;
      outfile<<"    <td align='center'>"<<std::endl;
      outfile<<"      <h2> StubMapper "<<name_s<<"</h2> "<<std::endl;
      outfile<<"      delay = "<<delay_s<<" ns <br/> "<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"    <td>"<<std::endl;
      outfile<<"      <table border='1'>"<<std::endl;
      outfile<<"        <tr>"<<std::endl;
      outfile<<"          <td>"<<std::endl;
      outfile<<"            <img src='"<<("c_"+name_s+"_0.png")<<"'/>"<<std::endl;
      outfile<<"          </td>"<<std::endl;
      outfile<<"          <td>"<<std::endl;
      outfile<<"            <img src='"<<("c_"+name_s+"_1.png")<<"'/>"<<std::endl;
      outfile<<"          </td>"<<std::endl;
      outfile<<"          <td>"<<std::endl;
      outfile<<"            <img src='"<<("c_"+name_s+"_2.png")<<"'/>"<<std::endl;
      outfile<<"          </td>"<<std::endl;
      outfile<<"          <td>"<<std::endl;
      outfile<<"            <img src='"<<("c_"+name_s+"_3.png")<<"'/>"<<std::endl;
      outfile<<"          </td>"<<std::endl;
      outfile<<"          <td>"<<std::endl;
      outfile<<"            <img src='"<<("c_"+name_s+"_4.png")<<"'/>"<<std::endl;
      outfile<<"          </td>"<<std::endl;
      outfile<<"          <td>"<<std::endl;
      outfile<<"            <img src='"<<("c_"+name_s+"_5.png")<<"'/>"<<std::endl;
      outfile<<"          </td>"<<std::endl;
      outfile<<"        </tr>"<<std::endl;
      outfile<<"      </table>"<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"  </tr>"<<std::endl;
      
      blockDiagram_0p50.addComponent("StubMapper_"+name_s+"_0", kGreen, h_t1out_sm_0, h_t2out_sm_0);
      blockDiagram_0p50.addComponent("StubMapper_"+name_s+"_1", kGreen, h_t1out_sm_1, h_t2out_sm_1);
      blockDiagram_0p50.addComponent("StubMapper_"+name_s+"_2", kGreen, h_t1out_sm_2, h_t2out_sm_2);
      blockDiagram_0p50.addComponent("StubMapper_"+name_s+"_3", kGreen, h_t1out_sm_3, h_t2out_sm_3);
      blockDiagram_0p50.addComponent("StubMapper_"+name_s+"_4", kGreen, h_t1out_sm_4, h_t2out_sm_4);
      blockDiagram_0p50.addComponent("StubMapper_"+name_s+"_5", kGreen, h_t1out_sm_5, h_t2out_sm_5);
      
      blockDiagram_0p999.addComponent("StubMapper_"+name_s+"_0", kGreen, h_t1out_sm_0, h_t2out_sm_0);
      blockDiagram_0p999.addComponent("StubMapper_"+name_s+"_1", kGreen, h_t1out_sm_1, h_t2out_sm_1);
      blockDiagram_0p999.addComponent("StubMapper_"+name_s+"_2", kGreen, h_t1out_sm_2, h_t2out_sm_2);
      blockDiagram_0p999.addComponent("StubMapper_"+name_s+"_3", kGreen, h_t1out_sm_3, h_t2out_sm_3);
      blockDiagram_0p999.addComponent("StubMapper_"+name_s+"_4", kGreen, h_t1out_sm_4, h_t2out_sm_4);
      blockDiagram_0p999.addComponent("StubMapper_"+name_s+"_5", kGreen, h_t1out_sm_5, h_t2out_sm_5);
    }
    
    if (component_s.find("AssociativeMemory")!=std::string::npos)
    {
      std::string name_s, delay_s, inTime_s, outTime_s;
      getline(ss, name_s, '|');
      getline(ss, delay_s, ',');
      getline(ss, inTime_s, ',');
      getline(ss, outTime_s, '|');
      removeSpaces(name_s);
      
      TFile *am=new TFile((""+name_s+".root").c_str());
      TH1F *h_t1out_am=(TH1F*)am->Get(("h_t1out_"+name_s).c_str());
      TH1F *h_t2out_am=(TH1F*)am->Get(("h_t2out_"+name_s).c_str());
      TH1F *h_nPatterns_am=(TH1F*)am->Get(("h_nPatterns_"+name_s).c_str());
      
      makeCanvas(h_nPatterns_am, name_s+"_nPatterns", "# Patterns", "", kRed);
      makeCanvas(h_t1out_am, h_t2out_am, name_s, "AssociativeMemory", "ns");
    
      outfile<<"  <tr>"<<std::endl;
      outfile<<"    <td align='center'>"<<std::endl;
      outfile<<"      <h2> AssociativeMemory "<<name_s<<"</h2> "<<std::endl;
      outfile<<"      delay = "<<delay_s<<" ns <br/> "<<std::endl;
      outfile<<"      inTime = "<<inTime_s<<" ns <br/> "<<std::endl;
      outfile<<"      outTime = "<<outTime_s<<" ns <br/> "<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"    <td>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+"_nPatterns.png")<<"'/>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+".png")<<"'/>"<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"  </tr>"<<std::endl;
      
      blockDiagram_0p50.addComponent("AssociativeMemory", kCyan, h_t1out_am, h_t2out_am);
      blockDiagram_0p999.addComponent("AssociativeMemory", kCyan, h_t1out_am, h_t2out_am);
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
      
      TFile *hb=new TFile((""+name_s+".root").c_str());
      TH1F *h_t1out_hb=(TH1F*)hb->Get(("h_t1out_"+name_s).c_str());
      TH1F *h_t2out_hb=(TH1F*)hb->Get(("h_t2out_"+name_s).c_str());
      TH1F *h_nOutwords_hb=(TH1F*)hb->Get(("h_nOutwords_"+name_s).c_str());
      
      makeCanvas(h_nOutwords_hb, name_s+"_nOutwords", "# Outwords", "", kRed);
      makeCanvas(h_t1out_hb, h_t2out_hb, name_s, "HitBuffer", "ns");
      
      outfile<<"  <tr>"<<std::endl;
      outfile<<"    <td align='center'>"<<std::endl;
      outfile<<"      <h2> HitBuffer "<<name_s<<"</h2> "<<std::endl;
      outfile<<"      inTime = "<<inTime_s<<" ns <br/> "<<std::endl;
      outfile<<"      procTime = "<<procTime_s<<" ns <br/> "<<std::endl;
      outfile<<"      outTime = "<<outTime_s<<" ns <br/> "<<std::endl;
      outfile<<"      delay = "<<delay_s<<" ns <br/> "<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"    <td>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+"_nOutwords.png")<<"'/>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+".png")<<"'/>"<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"  </tr>"<<std::endl;
      
      blockDiagram_0p50.addComponent("HitBuffer", kRed, h_t1out_hb, h_t2out_hb);
      blockDiagram_0p999.addComponent("HitBuffer", kRed, h_t1out_hb, h_t2out_hb);
    }
    
    if (component_s.find("CombinationBuilder")!=std::string::npos)
    {
      std::string name_s, inTime_s, outTime_s, delay_s;
      getline(ss, name_s, '|');
      getline(ss, inTime_s, ',');
      getline(ss, outTime_s, ',');
      getline(ss, delay_s, '|');
      removeSpaces(name_s);
      
      TFile *cb=new TFile((""+name_s+".root").c_str());
      TH1F *h_t1out_cb=(TH1F*)cb->Get(("h_t1out_"+name_s).c_str());
      TH1F *h_t2out_cb=(TH1F*)cb->Get(("h_t2out_"+name_s).c_str());
      TH1F *h_nCombinations_=(TH1F*)cb->Get(("h_nCombinations_"+name_s).c_str());
      
      makeCanvas(h_nCombinations_, name_s+"_nCombinations", "# Combinations", "", kRed);
      makeCanvas(h_t1out_cb, h_t2out_cb, name_s, "CombinationBuilder", "ns");
      
      outfile<<"  <tr>"<<std::endl;
      outfile<<"    <td align='center'>"<<std::endl;
      outfile<<"      <h2> CombinationBuilder "<<name_s<<"</h2> "<<std::endl;
      outfile<<"      inTime = "<<inTime_s<<" ns <br/> "<<std::endl;
      outfile<<"      outTime = "<<outTime_s<<" ns <br/> "<<std::endl;
      outfile<<"      delay = "<<delay_s<<" ns <br/> "<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"    <td>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+"_nCombinations.png")<<"'/>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+".png")<<"'/>"<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"  </tr>"<<std::endl;
      
      blockDiagram_0p50.addComponent("CombinationBuilder", kOrange, h_t1out_cb, h_t2out_cb);
      blockDiagram_0p999.addComponent("CombinationBuilder", kOrange, h_t1out_cb, h_t2out_cb);
    }
    
    if (component_s.find("TrackFitter")!=std::string::npos)
    {
      std::string name_s, inTime_s, outTime_s, delay_s;
      getline(ss, name_s, '|');
      getline(ss, inTime_s, ',');
      getline(ss, outTime_s, ',');
      getline(ss, delay_s, '|');
      removeSpaces(name_s);
      
      TFile *tf=new TFile((""+name_s+".root").c_str());
      TH1F *h_t1out_tf=(TH1F*)tf->Get(("h_t1out_"+name_s).c_str());
      TH1F *h_t2out_tf=(TH1F*)tf->Get(("h_t2out_"+name_s).c_str());
      TH1F *h_nTracks_=(TH1F*)tf->Get(("h_nTracks_"+name_s).c_str());
      
      makeCanvas(h_nTracks_, name_s+"_nTracks", "# Tracks", "", kRed);
      makeCanvas(h_t1out_tf, h_t2out_tf, name_s, "TrackFitter", "ns");
      
      outfile<<"  <tr>"<<std::endl;
      outfile<<"    <td align='center'>"<<std::endl;
      outfile<<"      <h2> TrackFitter "<<name_s<<"</h2> "<<std::endl;
      outfile<<"      inTime = "<<inTime_s<<" ns <br/> "<<std::endl;
      outfile<<"      outTime = "<<outTime_s<<" ns <br/> "<<std::endl;
      outfile<<"      delay = "<<delay_s<<" ns <br/> "<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"    <td>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+"_nTracks.png")<<"'/>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+".png")<<"'/>"<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"  </tr>"<<std::endl;
      
      blockDiagram_0p50.addComponent("TrackFitter", kViolet, h_t1out_tf, h_t2out_tf);
      blockDiagram_0p999.addComponent("TrackFitter", kViolet, h_t1out_tf, h_t2out_tf);
    }
  }
  
  outfile<<"</table>"<<std::endl;
  
  blockDiagram_0p50.drawCanvas();
  blockDiagram_0p999.drawCanvas();
  
  outfile<<" <hr/> "<<std::endl;
  outfile<<" <h1 align='center'> Summary Block Diagram </h2>"<<std::endl;
  outfile<<"  <img style='width:80\%' src='c_BlockDiagram_0.500.png'/>"<<std::endl;
  outfile<<"  <img style='width:80\%' src='c_BlockDiagram_0.999.png'/>"<<std::endl;
  outfile<<" <hr/> "<<std::endl;
  
  outfile<<"</body>"<<std::endl;
  outfile<<"</html>"<<std::endl;
}
  
  
  

