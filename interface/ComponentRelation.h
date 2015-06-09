#pragma once

#include "Component.h"

class ComponentRelation
{
  public:
    void extractComponentRelation(std::string);
    
  public:
    Component *comp_;
    std::vector<unsigned int> i_comp_;
    std::vector<unsigned int> i_output_;
    std::vector<unsigned int> i_input_;
};
