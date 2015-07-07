#include <string>
#include <vector>
#include "TCanvas.h"
#include "TPaveText.h"
#include "TGaxis.h"

class BlockDiagram
{

  public:
  
    BlockDiagram(double quantile){
      quantile_=quantile;
    }
    
    void addComponent(std::string name,
                      int color,
                      TH1F *h_t1out,
                      TH1F *h_t2out)
    {
      componentName_.push_back(name);
      color_.push_back(color);
      double perc[1]={quantile_};
      double t1out_perc[3];
      double t2out_perc[3];
      
      if (h_t1out!=0)
      {
        h_t1out->GetQuantiles(3, t1out_perc, perc);
        t1out_median_.push_back(t1out_perc[0]);
      }
      else
      {
        t1out_median_.push_back(0);
      }
      h_t2out->GetQuantiles(3, t2out_perc, perc);
      t2out_median_.push_back(t2out_perc[0]);
      t2out_95perc_.push_back(t2out_perc[1]);
      t2out_99p9perc_.push_back(t2out_perc[2]);
    }
    
    TCanvas* drawCanvas()
    {
      // Find the maximum t1out, t2out
      double max_tout=0;
      for (unsigned int i=0; i<componentName_.size(); ++i)
      {
        if (t1out_99p9perc_.at(i)>max_tout) max_tout=t1out_99p9perc_.at(i);
        if (t2out_99p9perc_.at(i)>max_tout) max_tout=t2out_99p9perc_.at(i);
      }
      
      // Now plot them
      double width=70./(2*componentName_.size()-1);
      double length=70./max_tout;
      TCanvas *c_BlockDiagram=new TCanvas("c_BlockDiagram", "c_BlockDiagram", 900, 700);
      c_BlockDiagram->Range(0, 0, 100, 100);
      for (unsigned int i=0; i<componentName_.size(); ++i)
      {
        TPaveText *label=new TPaveText(0, 15+(2*i)*width, 20, 15+(2*i+1)*width);
        label->AddText(componentName_.at(i).c_str());
        label->SetBorderSize(0);
        label->SetFillColor(0);
        label->Draw();
        TBox *box=new TBox(20+t1out_median_.at(i)*length, 15+(2*i)*width, 20+t2out_median_.at(i)*length, 15+(2*i+1)*width);
        box->SetFillColor(color_.at(i));
        box->Draw();
        TBox *box_95=new TBox(20+t1out_95perc_.at(i)*length, 15+(2*i)*width, 20+t2out_95perc_.at(i)*length, 15+(2*i+0.5)*width);
        box_95->SetFillStyle(0);
        box_95->SetLineColor(color_.at(i)+2);
        box_95->Draw();
        TBox *box_99p9=new TBox(20+t1out_99p9perc_.at(i)*length, 15+(2*i+0.5)*width, 20+t2out_99p9perc_.at(i)*length, 15+(2*i+1)*width);
        box_99p9->SetFillStyle(0);
        box_99p9->SetLineColor(color_.at(i)+3);
        box_99p9->Draw();
      }
      TPaveText *title=new TPaveText(30, 90, 80, 100);
      title->AddText("AM Mezzanine Board Timing");
      // title->InsertText("Empty Events");
      // title->InsertText("Single Muon sf1 nz1 TT27 6/6 Events");
      title->InsertText("MinBias 140 PU sf1 nz1 TT27 5/6 Events");
      title->SetBorderSize(0);
      title->SetFillColor(0);
      title->Draw();
      TGaxis *axis=new TGaxis(20, 10, 90, 10, 0, max_tout);
      axis->SetTitle("Output times (ns)");
      axis->Draw();
      TLine *line=new TLine(20, 10, 20, 90);
      line->Draw();
      c_BlockDiagram->SaveAs("c_BlockDiagram.png");
      return c_BlockDiagram;
    }
  
  private:
    double quantile_;
    std::vector<std::string> componentName_;
    std::vector<int> color_;
    std::vector<double> t1out_median_;
    std::vector<double> t2out_median_;
    
};
    
    
