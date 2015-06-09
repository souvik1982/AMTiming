#pragma once

#include <vector>
#include <string>

#include "EventCharacteristics.h"

class Component
{
  public:
    void set_t1in(unsigned int, double);
    void set_t2in(unsigned int, double);
    
    double get_t1out(unsigned int);
    double get_t2out(unsigned int);
    std::string get_name();
    void printOutputTimes();
    
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
};
