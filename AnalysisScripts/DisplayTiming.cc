#include <TH1F.h>
#include <THStack.h>
#include <TLegend.h>
#include <TArrow.h>
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

/*std::string ftoa3(double i) 
{
  char res[10];
  sprintf(res, "%2.3f", i);
  std::string ret(res);
  return ret;
}*/

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
  h->GetXaxis()->SetRange(0, h->FindLastBinAbove(0, 1)+10);
  return rebin;
}

void makeCanvas(TH1F *h1, double percentile, std::string name_s, std::string componentType, std::string units="", int color=kBlue)
{
  h1->SetLineColor(color);
  fixRange(h1);
  
  TCanvas *c=new TCanvas(("c_"+name_s).c_str(), ("c_"+name_s).c_str(), 700, 700);
  if (logScale) c->SetLogy();
  h1->SetTitle(("; "+componentType+" "+name_s+" ("+units+")").c_str());
  h1->Draw("");
  double perc[1]={percentile};
  double values[1];
  h1->GetQuantiles(1, values, perc);
  TLegend *leg=new TLegend(0.6, 0.7, 0.89, 0.89);
  leg->SetBorderSize(0);
  leg->AddEntry(h1, (ftoa3(percentile)+" perc: "+ftoa2(values[0])+" "+units).c_str());
  leg->Draw();
  TArrow *arrow=new TArrow(values[0], h1->GetMaximum()*0.5, values[0], 0, 0.05, "|>");
  arrow->Draw();
  c->SaveAs(("c_"+name_s+".png").c_str());
}

void makeCanvas(TH1F *h1, TH1F *h2, double percentile, std::string name_s, std::string componentType, std::string units="")
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
  double perc1[1]={percentile};
  double values1[1];
  h1->GetQuantiles(1, values1, perc1);
  double perc2[1]={percentile};
  double values2[1];
  h2->GetQuantiles(1, values2, perc2);
  TLegend *leg=new TLegend(0.6, 0.7, 0.89, 0.89);
  leg->SetBorderSize(0);
  leg->AddEntry(h1, ("t1out "+ftoa3(percentile)+" perc: "+ftoa2(values1[0])+" "+units).c_str());
  leg->AddEntry(h1, ("t2out "+ftoa3(percentile)+" perc: "+ftoa2(values2[0])+" "+units).c_str());
  leg->Draw();
  TArrow *arrow1=new TArrow(values1[0], h1->GetMaximum()*0.5, values1[0], 0, 0.05, "|>");
  arrow1->SetLineColor(kBlue); arrow1->Draw();
  TArrow *arrow2=new TArrow(values2[0], h2->GetMaximum()*0.5, values2[0], 0, 0.05, "|>");
  arrow2->SetLineColor(kGreen+2); arrow2->Draw();
  c->SaveAs(("c_"+name_s+".png").c_str());
}

TCanvas* makeCanvasForStack(TH1F *h1, double percentile, std::string name_s, std::string componentType, int &rebin, std::string units="", int color=kBlue)
{
  h1->SetLineColor(color);
  rebin=fixRange(h1);
  
  TCanvas *c=new TCanvas(("c_"+name_s).c_str(), ("c_"+name_s).c_str(), 700, 700);
  if (logScale) c->SetLogy();
  h1->SetTitle(("; "+componentType+" "+name_s+" ("+units+")").c_str());
  h1->Draw("");
  double perc[1]={percentile};
  double values[1];
  h1->GetQuantiles(1, values, perc);
  TLegend *leg=new TLegend(0.6, 0.8, 0.89, 0.89);
  leg->SetBorderSize(0);
  leg->AddEntry(h1, (ftoa3(percentile)+" perc: "+ftoa2(values[0])+" "+units).c_str());
  leg->Draw();
  TArrow *arrow=new TArrow(values[0], h1->GetMaximum()*0.5, values[0], 0, 0.05, "|>");
  arrow->Draw();
  c->SaveAs(("c_"+name_s+".png").c_str());
  
  return c;
}

void DisplayTiming(std::string title, std::string schematicFileName, double percentile)
{
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  
  BlockDiagram blockDiagram(percentile, title);
  
  ofstream outfile;
  outfile.open("index.html");
  outfile<<"<html>"<<std::endl;
  outfile<<"<head>"<<std::endl;
  outfile<<"</head>"<<std::endl;
  outfile<<"<body>"<<std::endl;
  outfile<<"<h1 align='center'> AM Mezzanine Board Timing Dashboard </h1>"<<std::endl;
  outfile<<"<table border='1'>"<<std::endl;
  
  std::ifstream file(schematicFileName.c_str());
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
      
      makeCanvas(h_t2out_ds_0, percentile, name_s+"_0", "Data Source", "ns");
      makeCanvas(h_t2out_ds_1, percentile, name_s+"_1", "Data Source", "ns");
      makeCanvas(h_t2out_ds_2, percentile, name_s+"_2", "Data Source", "ns");
      makeCanvas(h_t2out_ds_3, percentile, name_s+"_3", "Data Source", "ns");
      makeCanvas(h_t2out_ds_4, percentile, name_s+"_4", "Data Source", "ns");
      makeCanvas(h_t2out_ds_5, percentile, name_s+"_5", "Data Source", "ns");
      
      makeCanvas(h_nStubs_ds_0, percentile, name_s+"_nStubs_0", "Stub Occupancy", "", kRed);
      makeCanvas(h_nStubs_ds_1, percentile, name_s+"_nStubs_1", "Stub Occupancy", "", kRed);
      makeCanvas(h_nStubs_ds_2, percentile, name_s+"_nStubs_2", "Stub Occupancy", "", kRed);
      makeCanvas(h_nStubs_ds_3, percentile, name_s+"_nStubs_3", "Stub Occupancy", "", kRed);
      makeCanvas(h_nStubs_ds_4, percentile, name_s+"_nStubs_4", "Stub Occupancy", "", kRed);
      makeCanvas(h_nStubs_ds_5, percentile, name_s+"_nStubs_5", "Stub Occupancy", "", kRed);
      
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
      
      blockDiagram.addComponent("DataSource_"+name_s+"_0", kAzure+6, 0, h_t2out_ds_0);
      blockDiagram.addComponent("DataSource_"+name_s+"_1", kAzure+6, 0, h_t2out_ds_1);
      blockDiagram.addComponent("DataSource_"+name_s+"_2", kAzure+6, 0, h_t2out_ds_2);
      blockDiagram.addComponent("DataSource_"+name_s+"_3", kAzure+6, 0, h_t2out_ds_3);
      blockDiagram.addComponent("DataSource_"+name_s+"_4", kAzure+6, 0, h_t2out_ds_4);
      blockDiagram.addComponent("DataSource_"+name_s+"_5", kAzure+6, 0, h_t2out_ds_5);
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
      
      makeCanvas(h_t1out_sm_0, h_t2out_sm_0, percentile, name_s+"_0", "StubMapper", "ns");
      makeCanvas(h_t1out_sm_1, h_t2out_sm_1, percentile, name_s+"_1", "StubMapper", "ns");
      makeCanvas(h_t1out_sm_2, h_t2out_sm_2, percentile, name_s+"_2", "StubMapper", "ns");
      makeCanvas(h_t1out_sm_3, h_t2out_sm_3, percentile, name_s+"_3", "StubMapper", "ns");
      makeCanvas(h_t1out_sm_4, h_t2out_sm_4, percentile, name_s+"_4", "StubMapper", "ns");
      makeCanvas(h_t1out_sm_5, h_t2out_sm_5, percentile, name_s+"_5", "StubMapper", "ns");
      
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
      
      blockDiagram.addComponent("StubMapper_"+name_s+"_0", kGreen, h_t1out_sm_0, h_t2out_sm_0);
      blockDiagram.addComponent("StubMapper_"+name_s+"_1", kGreen, h_t1out_sm_1, h_t2out_sm_1);
      blockDiagram.addComponent("StubMapper_"+name_s+"_2", kGreen, h_t1out_sm_2, h_t2out_sm_2);
      blockDiagram.addComponent("StubMapper_"+name_s+"_3", kGreen, h_t1out_sm_3, h_t2out_sm_3);
      blockDiagram.addComponent("StubMapper_"+name_s+"_4", kGreen, h_t1out_sm_4, h_t2out_sm_4);
      blockDiagram.addComponent("StubMapper_"+name_s+"_5", kGreen, h_t1out_sm_5, h_t2out_sm_5);
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
      TH1F *h_t1out_term1_am_0=(TH1F*)am->Get(("h_t1out_term1_"+name_s+"_0").c_str());
      TH1F *h_t1out_term1_am_1=(TH1F*)am->Get(("h_t1out_term1_"+name_s+"_1").c_str());
      TH1F *h_t1out_term1_am_2=(TH1F*)am->Get(("h_t1out_term1_"+name_s+"_2").c_str());
      TH1F *h_t1out_term1_am_3=(TH1F*)am->Get(("h_t1out_term1_"+name_s+"_3").c_str());
      TH1F *h_t1out_term1_am_4=(TH1F*)am->Get(("h_t1out_term1_"+name_s+"_4").c_str());
      TH1F *h_t1out_term1_am_5=(TH1F*)am->Get(("h_t1out_term1_"+name_s+"_5").c_str());
      TH1F *h_t1out_term2_am_0=(TH1F*)am->Get(("h_t1out_term2_"+name_s+"_0").c_str());
      TH1F *h_t1out_term2_am_1=(TH1F*)am->Get(("h_t1out_term2_"+name_s+"_1").c_str());
      TH1F *h_t1out_term2_am_2=(TH1F*)am->Get(("h_t1out_term2_"+name_s+"_2").c_str());
      TH1F *h_t1out_term2_am_3=(TH1F*)am->Get(("h_t1out_term2_"+name_s+"_3").c_str());
      TH1F *h_t1out_term2_am_4=(TH1F*)am->Get(("h_t1out_term2_"+name_s+"_4").c_str());
      TH1F *h_t1out_term2_am_5=(TH1F*)am->Get(("h_t1out_term2_"+name_s+"_5").c_str());
      TH1F *h_nPatterns_am=(TH1F*)am->Get(("h_nPatterns_"+name_s).c_str());
      
      makeCanvas(h_nPatterns_am, percentile, name_s+"_nPatterns", "# Patterns", "", kRed);
      
      double am_t1out_total=h_t1out_am->GetSumOfWeights();
      double am_t1out_term1_layer_0=h_t1out_term1_am_0->GetSumOfWeights()/am_t1out_total;
      double am_t1out_term1_layer_1=h_t1out_term1_am_1->GetSumOfWeights()/am_t1out_total;
      double am_t1out_term1_layer_2=h_t1out_term1_am_2->GetSumOfWeights()/am_t1out_total;
      double am_t1out_term1_layer_3=h_t1out_term1_am_3->GetSumOfWeights()/am_t1out_total;
      double am_t1out_term1_layer_4=h_t1out_term1_am_4->GetSumOfWeights()/am_t1out_total;
      double am_t1out_term1_layer_5=h_t1out_term1_am_5->GetSumOfWeights()/am_t1out_total;
      double am_t1out_term2_layer_0=h_t1out_term2_am_0->GetSumOfWeights()/am_t1out_total;
      double am_t1out_term2_layer_1=h_t1out_term2_am_1->GetSumOfWeights()/am_t1out_total;
      double am_t1out_term2_layer_2=h_t1out_term2_am_2->GetSumOfWeights()/am_t1out_total;
      double am_t1out_term2_layer_3=h_t1out_term2_am_3->GetSumOfWeights()/am_t1out_total;
      double am_t1out_term2_layer_4=h_t1out_term2_am_4->GetSumOfWeights()/am_t1out_total;
      double am_t1out_term2_layer_5=h_t1out_term2_am_5->GetSumOfWeights()/am_t1out_total;
      int rebin=0;
      TCanvas *c_t1out_am=makeCanvasForStack(h_t1out_am, percentile, name_s+"_t1out", "AssociativeMemory", rebin, "ns");
      h_t1out_term1_am_0->Rebin(rebin);
      h_t1out_term1_am_1->Rebin(rebin);
      h_t1out_term1_am_2->Rebin(rebin);
      h_t1out_term1_am_3->Rebin(rebin);
      h_t1out_term1_am_4->Rebin(rebin);
      h_t1out_term1_am_5->Rebin(rebin);
      h_t1out_term2_am_0->Rebin(rebin);
      h_t1out_term2_am_1->Rebin(rebin);
      h_t1out_term2_am_2->Rebin(rebin);
      h_t1out_term2_am_3->Rebin(rebin);
      h_t1out_term2_am_4->Rebin(rebin);
      h_t1out_term2_am_5->Rebin(rebin);
      h_t1out_term1_am_0->SetFillColor(kBlue);
      h_t1out_term1_am_1->SetFillColor(kGreen);
      h_t1out_term1_am_2->SetFillColor(kCyan);
      h_t1out_term1_am_3->SetFillColor(kOrange);
      h_t1out_term1_am_4->SetFillColor(kRed);
      h_t1out_term1_am_5->SetFillColor(kViolet);
      h_t1out_term2_am_0->SetFillColor(kBlue+2);
      h_t1out_term2_am_1->SetFillColor(kGreen+2);
      h_t1out_term2_am_2->SetFillColor(kCyan+2);
      h_t1out_term2_am_3->SetFillColor(kOrange+2);
      h_t1out_term2_am_4->SetFillColor(kRed+2);
      h_t1out_term2_am_5->SetFillColor(kViolet+2);
      THStack *s_t1out_am=new THStack("s_t1out_am", "");
      s_t1out_am->Add(h_t1out_term1_am_5);
      s_t1out_am->Add(h_t1out_term1_am_4);
      s_t1out_am->Add(h_t1out_term1_am_3);
      s_t1out_am->Add(h_t1out_term1_am_2);
      s_t1out_am->Add(h_t1out_term1_am_1);
      s_t1out_am->Add(h_t1out_term1_am_0);
      s_t1out_am->Add(h_t1out_term2_am_5);
      s_t1out_am->Add(h_t1out_term2_am_4);
      s_t1out_am->Add(h_t1out_term2_am_3);
      s_t1out_am->Add(h_t1out_term2_am_2);
      s_t1out_am->Add(h_t1out_term2_am_1);
      s_t1out_am->Add(h_t1out_term2_am_0);
      s_t1out_am->Draw("same");
      h_t1out_am->Draw("same");
      TLegend *leg=new TLegend(0.7, 0.5, 0.89, 0.79);
      leg->AddEntry(h_t1out_term1_am_0, "layer 0 term 1");
      leg->AddEntry(h_t1out_term1_am_1, "layer 1 term 1");
      leg->AddEntry(h_t1out_term1_am_2, "layer 2 term 1");
      leg->AddEntry(h_t1out_term1_am_3, "layer 3 term 1");
      leg->AddEntry(h_t1out_term1_am_4, "layer 4 term 1");
      leg->AddEntry(h_t1out_term1_am_5, "layer 5 term 1");
      leg->AddEntry(h_t1out_term2_am_0, "layer 0 term 2");
      leg->AddEntry(h_t1out_term2_am_1, "layer 1 term 2");
      leg->AddEntry(h_t1out_term2_am_2, "layer 2 term 2");
      leg->AddEntry(h_t1out_term2_am_3, "layer 3 term 2");
      leg->AddEntry(h_t1out_term2_am_4, "layer 4 term 2");
      leg->AddEntry(h_t1out_term2_am_5, "layer 5 term 2");
      leg->SetLineColor(kWhite);
      leg->Draw("same");
      c_t1out_am->SaveAs(("c_"+name_s+"_t1out.png").c_str());
      
      makeCanvas(h_t2out_am, percentile, name_s+"_t2out", "AssociativeMemory", "ns");
    
      outfile<<"  <tr>"<<std::endl;
      outfile<<"    <td align='center'>"<<std::endl;
      outfile<<"      <h2> AssociativeMemory "<<name_s<<"</h2> "<<std::endl;
      outfile<<"      delay = "<<delay_s<<" ns <br/> "<<std::endl;
      outfile<<"      inTime = "<<inTime_s<<" ns <br/> "<<std::endl;
      outfile<<"      outTime = "<<outTime_s<<" ns <br/> "<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"    <td>"<<std::endl;
      outfile<<"      <table border='1'>"<<std::endl;
      outfile<<"        <tr>"<<std::endl;
      outfile<<"          <td align='center'>"<<std::endl;
      outfile<<"            <h2> nPatterns </h2>"<<std::endl;
      outfile<<"            <img src='"<<("c_"+name_s+"_nPatterns.png")<<"'/>"<<std::endl;
      outfile<<"          </td>"<<std::endl;
      outfile<<"          <td align='center'>"<<std::endl;
      outfile<<"            <h2> t1out </h2>"<<std::endl;
      outfile<<"            <img src='"<<("c_"+name_s+"_t1out.png")<<"'/>"<<std::endl;
      outfile<<"            <table border='1' align='center'>"<<std::endl;
      outfile<<"              <tr>"<<std::endl;
      outfile<<"                <td> Layer </td> <td> term 1: t2in(SM) </td> <td> term2: (nStubs+1)*inTime </td>"<<std::endl;
      outfile<<"              </tr>"<<std::endl;
      outfile<<"              <tr><td> 0 </td> <td> "<<am_t1out_term1_layer_0*100.<<"\% </td> <td> "<<am_t1out_term2_layer_0*100.<<"\% </td></tr>"<<std::endl;
      outfile<<"              <tr><td> 1 </td> <td> "<<am_t1out_term1_layer_1*100.<<"\% </td> <td> "<<am_t1out_term2_layer_1*100.<<"\% </td></tr>"<<std::endl;
      outfile<<"              <tr><td> 2 </td> <td> "<<am_t1out_term1_layer_2*100.<<"\% </td> <td> "<<am_t1out_term2_layer_2*100.<<"\% </td></tr>"<<std::endl;
      outfile<<"              <tr><td> 3 </td> <td> "<<am_t1out_term1_layer_3*100.<<"\% </td> <td> "<<am_t1out_term2_layer_3*100.<<"\% </td></tr>"<<std::endl;
      outfile<<"              <tr><td> 4 </td> <td> "<<am_t1out_term1_layer_4*100.<<"\% </td> <td> "<<am_t1out_term2_layer_4*100.<<"\% </td></tr>"<<std::endl;
      outfile<<"              <tr><td> 5 </td> <td> "<<am_t1out_term1_layer_5*100.<<"\% </td> <td> "<<am_t1out_term2_layer_5*100.<<"\% </td></tr>"<<std::endl;
      outfile<<"            </table>"<<std::endl;
      outfile<<"          </td>"<<std::endl;
      outfile<<"          <td align='center'>"<<std::endl;
      outfile<<"            <h2> t2out </h2>"<<std::endl;
      outfile<<"            <img src='"<<("c_"+name_s+"_t2out.png")<<"'/>"<<std::endl;
      outfile<<"          </td>"<<std::endl;
      outfile<<"        </tr>"<<std::endl;
      outfile<<"      </table>"<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"  </tr>"<<std::endl;
      
      blockDiagram.addComponent("AssociativeMemory", kCyan, h_t1out_am, h_t2out_am);
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
      
      makeCanvas(h_nOutwords_hb, percentile, name_s+"_nOutwords", "# Outwords", "", kRed);
      makeCanvas(h_t1out_hb, h_t2out_hb, percentile, name_s, "HitBuffer", "ns");
      
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
      
      blockDiagram.addComponent("HitBuffer", kRed, h_t1out_hb, h_t2out_hb);
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
      
      makeCanvas(h_nCombinations_, percentile, name_s+"_nCombinations", "# Combinations", "", kRed);
      makeCanvas(h_t1out_cb, h_t2out_cb, percentile, name_s, "CombinationBuilder", "ns");
      
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
      
      blockDiagram.addComponent("CombinationBuilder", kOrange, h_t1out_cb, h_t2out_cb);
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
      
      makeCanvas(h_nTracks_, percentile, name_s+"_nTracks", "# Tracks", "", kRed);
      makeCanvas(h_t1out_tf, h_t2out_tf, percentile, name_s, "TrackFitter", "ns");
      
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
      
      blockDiagram.addComponent("TrackFitter", kViolet, h_t1out_tf, h_t2out_tf);
    }
  }
  
  outfile<<"</table>"<<std::endl;
  
  blockDiagram.drawCanvas();
  
  outfile<<" <hr/> "<<std::endl;
  outfile<<" <h1 align='center'> Summary Block Diagram </h1>"<<std::endl;
  outfile<<"  <img style='width:80\%' src='"<<("c_BlockDiagram_"+ftoa3(percentile)+".png").c_str()<<"'/>"<<std::endl;
  outfile<<" <hr/> "<<std::endl;
  /*
  TFile *f_fifo=new TFile("fifos.root");
  TH1F *h_AM_FIFO_0=(TH1F*)f_fifo->Get("h_AM_FIFO_0");
  TH1F *h_AM_FIFO_1=(TH1F*)f_fifo->Get("h_AM_FIFO_1");
  TH1F *h_AM_FIFO_2=(TH1F*)f_fifo->Get("h_AM_FIFO_2");
  TH1F *h_AM_FIFO_3=(TH1F*)f_fifo->Get("h_AM_FIFO_3");
  TH1F *h_AM_FIFO_4=(TH1F*)f_fifo->Get("h_AM_FIFO_4");
  TH1F *h_AM_FIFO_5=(TH1F*)f_fifo->Get("h_AM_FIFO_5");
  makeCanvas(h_AM_FIFO_0, "fifo1_0", "FIFO Layer 0 Depth", "", kBlack);
  makeCanvas(h_AM_FIFO_1, "fifo1_1", "FIFO Layer 1 Depth", "", kBlack);
  makeCanvas(h_AM_FIFO_2, "fifo1_2", "FIFO Layer 2 Depth", "", kBlack);
  makeCanvas(h_AM_FIFO_3, "fifo1_3", "FIFO Layer 3 Depth", "", kBlack);
  makeCanvas(h_AM_FIFO_4, "fifo1_4", "FIFO Layer 4 Depth", "", kBlack);
  makeCanvas(h_AM_FIFO_5, "fifo1_5", "FIFO Layer 5 Depth", "", kBlack);
  outfile<<" <h1 align='center'> AM FIFO Depth Analysis </h1>"<<std::endl;
  outfile<<"  <img style='width:80\%' src='c_fifo1_0.png'/>"<<std::endl;
  outfile<<"  <img style='width:80\%' src='c_fifo1_1.png'/>"<<std::endl;
  outfile<<"  <img style='width:80\%' src='c_fifo1_2.png'/>"<<std::endl;
  outfile<<"  <img style='width:80\%' src='c_fifo1_3.png'/>"<<std::endl;
  outfile<<"  <img style='width:80\%' src='c_fifo1_4.png'/>"<<std::endl;
  outfile<<"  <img style='width:80\%' src='c_fifo1_5.png'/>"<<std::endl;
  outfile<<" <hr/> "<<std::endl;
  */
  outfile<<"</body>"<<std::endl;
  outfile<<"</html>"<<std::endl;
}
  
  
  

