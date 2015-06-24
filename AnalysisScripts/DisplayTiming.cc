#include <iostream>

bool logScale=true;

void removeSpaces(std::string &input)
{
  input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
}

void DisplayTiming()
{
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  
  ofstream outfile;
  outfile.open("AMTimingAnalysis.html");
  outfile<<"<html>"<<std::endl;
  outfile<<"<head>"<<std::endl;
  outfile<<"</head>"<<std::endl;
  outfile<<"<body>"<<std::endl;
  outfile<<"<h1> AM Mezzanine Board Timing Analysis </h1>"<<std::endl;
  outfile<<"<table border='1'>"<<std::endl;
  
  std::ifstream file("../Schematic.txt");
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
      
      TFile *ds=new TFile(("../"+name_s+".root").c_str());
      TH1F *h_t2out_ds_0=(TH1F*)ds->Get(("h_t2out_"+name_s+"_0").c_str());
      TH1F *h_t2out_ds_1=(TH1F*)ds->Get(("h_t2out_"+name_s+"_1").c_str());
      TH1F *h_t2out_ds_2=(TH1F*)ds->Get(("h_t2out_"+name_s+"_2").c_str());
      TH1F *h_t2out_ds_3=(TH1F*)ds->Get(("h_t2out_"+name_s+"_3").c_str());
      TH1F *h_t2out_ds_4=(TH1F*)ds->Get(("h_t2out_"+name_s+"_4").c_str());
      TH1F *h_t2out_ds_5=(TH1F*)ds->Get(("h_t2out_"+name_s+"_5").c_str());
      h_t2out_ds_0->GetXaxis()->SetRangeUser(0, 500);
      h_t2out_ds_1->GetXaxis()->SetRangeUser(0, 500);
      h_t2out_ds_2->GetXaxis()->SetRangeUser(0, 500);
      h_t2out_ds_3->GetXaxis()->SetRangeUser(0, 500);
      h_t2out_ds_4->GetXaxis()->SetRangeUser(0, 500);
      h_t2out_ds_5->GetXaxis()->SetRangeUser(0, 500);
      TCanvas *c_ds_0=new TCanvas("c_ds_0", "c_ds_0", 700, 700);
      if (logScale) c_ds_0->SetLogy();
      h_t2out_ds_0->Draw();
      c_ds_0->SaveAs(("c_"+name_s+"_0.png").c_str());
      TCanvas *c_ds_1=new TCanvas("c_ds_1", "c_ds_1", 700, 700);
      if (logScale) c_ds_1->SetLogy();
      h_t2out_ds_1->Draw();
      c_ds_1->SaveAs(("c_"+name_s+"_1.png").c_str());
      TCanvas *c_ds_2=new TCanvas("c_ds_2", "c_ds_2", 700, 700);
      if (logScale) c_ds_2->SetLogy();
      h_t2out_ds_2->Draw();
      c_ds_2->SaveAs(("c_"+name_s+"_2.png").c_str());
      TCanvas *c_ds_3=new TCanvas("c_ds_3", "c_ds_3", 700, 700);
      if (logScale) c_ds_3->SetLogy();
      h_t2out_ds_3->Draw();
      c_ds_3->SaveAs(("c_"+name_s+"_3.png").c_str());
      TCanvas *c_ds_4=new TCanvas("c_ds_4", "c_ds_4", 700, 700);
      if (logScale) c_ds_4->SetLogy();
      h_t2out_ds_4->Draw();
      c_ds_4->SaveAs(("c_"+name_s+"_4.png").c_str());
      TCanvas *c_ds_5=new TCanvas("c_ds_5", "c_ds_5", 700, 700);
      if (logScale) c_ds_5->SetLogy();
      h_t2out_ds_5->Draw();
      c_ds_5->SaveAs(("c_"+name_s+"_5.png").c_str());
      
      outfile<<"  <tr>"<<std::endl;
      outfile<<"    <td align='center'>"<<std::endl;
      outfile<<"      <h2> DataSource "<<name_s<<"</h2> "<<std::endl;
      outfile<<"      outTime = "<<outTime_s<<" ns <br/> "<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"    <td>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+"_0.png")<<"'/>"<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"    <td>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+"_1.png")<<"'/>"<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"    <td>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+"_2.png")<<"'/>"<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"    <td>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+"_3.png")<<"'/>"<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"    <td>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+"_4.png")<<"'/>"<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"    <td>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+"_5.png")<<"'/>"<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"  </tr>"<<std::endl;
    }
    
    if (component_s.find("StubMapper")!=std::string::npos)
    {
      std::string name_s, delay_s;
      getline(ss, name_s, '|');
      getline(ss, delay_s, '|');
      removeSpaces(name_s);
      
      TFile *sm=new TFile(("../"+name_s+".root").c_str());
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
      h_t2out_sm_0->SetLineColor(kGreen+2);
      h_t2out_sm_1->SetLineColor(kGreen+2);
      h_t2out_sm_2->SetLineColor(kGreen+2);
      h_t2out_sm_3->SetLineColor(kGreen+2);
      h_t2out_sm_4->SetLineColor(kGreen+2);
      h_t2out_sm_5->SetLineColor(kGreen+2);
      h_t1out_sm_0->GetXaxis()->SetRangeUser(0, 500);
      h_t1out_sm_1->GetXaxis()->SetRangeUser(0, 500);
      h_t1out_sm_2->GetXaxis()->SetRangeUser(0, 500);
      h_t1out_sm_3->GetXaxis()->SetRangeUser(0, 500);
      h_t1out_sm_4->GetXaxis()->SetRangeUser(0, 500);
      h_t1out_sm_5->GetXaxis()->SetRangeUser(0, 500);
      h_t2out_sm_0->GetXaxis()->SetRangeUser(0, 500);
      h_t2out_sm_1->GetXaxis()->SetRangeUser(0, 500);
      h_t2out_sm_2->GetXaxis()->SetRangeUser(0, 500);
      h_t2out_sm_3->GetXaxis()->SetRangeUser(0, 500);
      h_t2out_sm_4->GetXaxis()->SetRangeUser(0, 500);
      h_t2out_sm_5->GetXaxis()->SetRangeUser(0, 500);
      TCanvas *c_sm_0=new TCanvas("c_sm_0", "c_sm_0", 700, 700);
      if (logScale) c_sm_0->SetLogy();
      h_t1out_sm_0->SetTitle(("; StubMapper "+name_s+" layer 0 (ns)").c_str());
      h_t1out_sm_0->Draw();
      h_t2out_sm_0->Draw("same");
      c_sm_0->SaveAs(("c_"+name_s+"_0.png").c_str());
      TCanvas *c_sm_1=new TCanvas("c_sm_1", "c_sm_1", 700, 700);
      if (logScale) c_sm_1->SetLogy();
      h_t1out_sm_1->SetTitle(("; StubMapper "+name_s+" layer 1 (ns)").c_str());
      h_t1out_sm_1->Draw();
      h_t2out_sm_1->Draw("same");
      c_sm_1->SaveAs(("c_"+name_s+"_1.png").c_str());
      TCanvas *c_sm_2=new TCanvas("c_sm_2", "c_sm_2", 700, 700);
      if (logScale) c_sm_2->SetLogy();
      h_t1out_sm_2->SetTitle(("; StubMapper "+name_s+" layer 2 (ns)").c_str());
      h_t1out_sm_2->Draw();
      h_t2out_sm_2->Draw("same");
      c_sm_2->SaveAs(("c_"+name_s+"_2.png").c_str());
      TCanvas *c_sm_3=new TCanvas("c_sm_3", "c_sm_3", 700, 700);
      if (logScale) c_sm_3->SetLogy();
      h_t1out_sm_3->SetTitle(("; StubMapper "+name_s+" layer 3 (ns)").c_str());
      h_t1out_sm_3->Draw();
      h_t2out_sm_3->Draw("same");
      c_sm_3->SaveAs(("c_"+name_s+"_3.png").c_str());
      TCanvas *c_sm_4=new TCanvas("c_sm_4", "c_sm_4", 700, 700);
      if (logScale) c_sm_4->SetLogy();
      h_t1out_sm_4->SetTitle(("; StubMapper "+name_s+" layer 4 (ns)").c_str());
      h_t1out_sm_4->Draw();
      h_t2out_sm_4->Draw("same");
      c_sm_4->SaveAs(("c_"+name_s+"_4.png").c_str());
      TCanvas *c_sm_5=new TCanvas("c_sm_5", "c_sm_5", 700, 700);
      if (logScale) c_sm_5->SetLogy();
      h_t1out_sm_5->SetTitle(("; StubMapper "+name_s+" layer 5 (ns)").c_str());
      h_t1out_sm_5->Draw();
      h_t2out_sm_5->Draw("same");
      c_sm_5->SaveAs(("c_"+name_s+"_5.png").c_str());
      
      outfile<<"  <tr>"<<std::endl;
      outfile<<"    <td align='center'>"<<std::endl;
      outfile<<"      <h2> StubMapper "<<name_s<<"</h2> "<<std::endl;
      outfile<<"      delay = "<<delay_s<<" ns <br/> "<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"    <td>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+"_0.png")<<"'/>"<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"    <td>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+"_1.png")<<"'/>"<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"    <td>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+"_2.png")<<"'/>"<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"    <td>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+"_3.png")<<"'/>"<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"    <td>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+"_4.png")<<"'/>"<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"    <td>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+"_5.png")<<"'/>"<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"  </tr>"<<std::endl;
    }
    
    if (component_s.find("AssociativeMemory")!=std::string::npos)
    {
      std::string name_s, delay_s, inTime_s, outTime_s;
      getline(ss, name_s, '|');
      getline(ss, delay_s, ',');
      getline(ss, inTime_s, ',');
      getline(ss, outTime_s, '|');
      removeSpaces(name_s);
      
      TFile *am=new TFile(("../"+name_s+".root").c_str());
      TH1F *h_t1out_am=(TH1F*)am->Get(("h_t1out_"+name_s).c_str());
      TH1F *h_t2out_am=(TH1F*)am->Get(("h_t2out_"+name_s).c_str());
      h_t2out_am->SetLineColor(kGreen+2);
      TCanvas *c_am=new TCanvas("c_am", "c_am", 700, 700);
      if (logScale) c_am->SetLogy();
      h_t1out_am->SetTitle(("; AssociativeMemory "+name_s+" (ns)").c_str());
      h_t1out_am->Draw();
      h_t2out_am->Draw("same");
      c_am->SaveAs(("c_"+name_s+".png").c_str());
    
      outfile<<"  <tr>"<<std::endl;
      outfile<<"    <td align='center'>"<<std::endl;
      outfile<<"      <h2> AssociativeMemory "<<name_s<<"</h2> "<<std::endl;
      outfile<<"      delay = "<<delay_s<<" ns <br/> "<<std::endl;
      outfile<<"      inTime = "<<inTime_s<<" ns <br/> "<<std::endl;
      outfile<<"      outTime = "<<outTime_s<<" ns <br/> "<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"    <td>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+".png")<<"'/>"<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"  </tr>"<<std::endl;
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
      
      TFile *hb=new TFile(("../"+name_s+".root").c_str());
      TH1F *h_t1out_hb=(TH1F*)hb->Get(("h_t1out_"+name_s).c_str());
      TH1F *h_t2out_hb=(TH1F*)hb->Get(("h_t2out_"+name_s).c_str());
      h_t2out_hb->SetLineColor(kGreen+2);
      TCanvas *c_hb=new TCanvas("c_hb", "c_hb", 700, 700);
      if (logScale) c_hb->SetLogy();
      h_t1out_hb->SetTitle(("; HitBuffer "+name_s+" (ns)").c_str());
      h_t1out_hb->Draw();
      h_t2out_hb->Draw("same");
      c_hb->SaveAs(("c_"+name_s+".png").c_str());
      
      outfile<<"  <tr>"<<std::endl;
      outfile<<"    <td align='center'>"<<std::endl;
      outfile<<"      <h2> HitBuffer "<<name_s<<"</h2> "<<std::endl;
      outfile<<"      inTime = "<<inTime_s<<" ns <br/> "<<std::endl;
      outfile<<"      procTime = "<<procTime_s<<" ns <br/> "<<std::endl;
      outfile<<"      outTime = "<<outTime_s<<" ns <br/> "<<std::endl;
      outfile<<"      delay = "<<delay_s<<" ns <br/> "<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"    <td>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+".png")<<"'/>"<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"  </tr>"<<std::endl;
    }
    
    if (component_s.find("CombinationBuilder")!=std::string::npos)
    {
      std::string name_s, inTime_s, outTime_s, delay_s;
      getline(ss, name_s, '|');
      getline(ss, inTime_s, ',');
      getline(ss, outTime_s, ',');
      getline(ss, delay_s, '|');
      removeSpaces(name_s);
      
      TFile *cb=new TFile(("../"+name_s+".root").c_str());
      TH1F *h_t1out_cb=(TH1F*)cb->Get(("h_t1out_"+name_s).c_str());
      TH1F *h_t2out_cb=(TH1F*)cb->Get(("h_t2out_"+name_s).c_str());
      h_t2out_cb->SetLineColor(kGreen+2);
      TCanvas *c_cb=new TCanvas("c_cb", "c_cb", 700, 700);
      if (logScale) c_cb->SetLogy();
      h_t1out_cb->SetTitle(("; CombinationBuilder "+name_s+" (ns)").c_str());
      h_t1out_cb->Draw();
      h_t2out_cb->Draw("same");
      c_cb->SaveAs(("c_"+name_s+".png").c_str());
      
      outfile<<"  <tr>"<<std::endl;
      outfile<<"    <td align='center'>"<<std::endl;
      outfile<<"      <h2> CombinationBuilder "<<name_s<<"</h2> "<<std::endl;
      outfile<<"      inTime = "<<inTime_s<<" ns <br/> "<<std::endl;
      outfile<<"      outTime = "<<outTime_s<<" ns <br/> "<<std::endl;
      outfile<<"      delay = "<<delay_s<<" ns <br/> "<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"    <td>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+".png")<<"'/>"<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"  </tr>"<<std::endl;
    }
    
    if (component_s.find("TrackFitter")!=std::string::npos)
    {
      std::string name_s, inTime_s, outTime_s, delay_s;
      getline(ss, name_s, '|');
      getline(ss, inTime_s, ',');
      getline(ss, outTime_s, ',');
      getline(ss, delay_s, '|');
      removeSpaces(name_s);
      
      TFile *tf=new TFile(("../"+name_s+".root").c_str());
      TH1F *h_t1out_tf=(TH1F*)tf->Get(("h_t1out_"+name_s).c_str());
      TH1F *h_t2out_tf=(TH1F*)tf->Get(("h_t2out_"+name_s).c_str());
      h_t2out_tf->SetLineColor(kGreen+2);
      TCanvas *c_tf=new TCanvas("c_tf", "c_tf", 700, 700);
      if (logScale) c_tf->SetLogy();
      h_t1out_tf->SetTitle(("; CombinationBuilder "+name_s+" (ns)").c_str());
      h_t1out_tf->Draw();
      h_t2out_tf->Draw("same");
      c_tf->SaveAs(("c_"+name_s+".png").c_str());
      
      outfile<<"  <tr>"<<std::endl;
      outfile<<"    <td align='center'>"<<std::endl;
      outfile<<"      <h2> TrackFitter "<<name_s<<"</h2> "<<std::endl;
      outfile<<"      inTime = "<<inTime_s<<" ns <br/> "<<std::endl;
      outfile<<"      outTime = "<<outTime_s<<" ns <br/> "<<std::endl;
      outfile<<"      delay = "<<delay_s<<" ns <br/> "<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"    <td>"<<std::endl;
      outfile<<"      <img src='"<<("c_"+name_s+".png")<<"'/>"<<std::endl;
      outfile<<"    </td>"<<std::endl;
      outfile<<"  </tr>"<<std::endl;
    }
  }
  
  outfile<<"</table>"<<std::endl;
  outfile<<"</body>"<<std::endl;
  outfile<<"</html>"<<std::endl;
}
  
  
  

