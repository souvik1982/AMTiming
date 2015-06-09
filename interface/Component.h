#pragma once

#include <vector>
#include <string>

#include "TH1F.h"
#include "TFile.h"

#include "EventCharacteristics.h"
#include "ToolBox.h"

class Component
{
  public:
    void set_t1in(unsigned int, double);
    void set_t2in(unsigned int, double);
    
    double get_t1out(unsigned int);
    double get_t2out(unsigned int);
    std::string get_name();
    void printOutputTimes();
    void writeOutputTimes();
    void drawOutputTimes();
    
    void clearValues();
    
    virtual bool setEventCharacteristics(EventCharacteristics*) = 0;
    virtual bool computeOutputTimes() = 0;
  
  protected:
    int nInputs_;
    int nOutputs_;
    std::vector<double> t1in_;
    std::vector<double> t2in_;
    std::vector<double> t1out_;
    std::vector<double> t2out_;
    std::string name_;
    std::vector<TH1F*> v_h_t1out_;
    std::vector<TH1F*> v_h_t2out_;
};
