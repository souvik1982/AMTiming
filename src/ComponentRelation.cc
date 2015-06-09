#include <stdlib.h>
#include <iostream>
#include <sstream>

#include "../interface/ComponentRelation.h"

void ComponentRelation::extractComponentRelation(std::string s)
{
  std::stringstream ss(s);
  std::string buf;
  getline(ss, buf, '|');
  getline(ss, buf, '|');
  
  int status=0;
  unsigned int i_comp, i_output, i_input;
  while (ss)
  {
    std::string s;
    if (status==0 && getline(ss, s, '('))
    {
      status=1;
    }
    if (status==1 && getline(ss, s, ','))
    {
      i_comp=atoi(s.c_str());
      status=2;
    }
    if (status==2 && getline(ss, s, ','))
    {
      i_output=atoi(s.c_str());
      status=3;
    }
    if (status==3 && getline(ss, s, ')'))
    {
      i_input=atoi(s.c_str());
      i_comp_.push_back(i_comp);
      i_output_.push_back(i_output);
      i_input_.push_back(i_input);
      status=0;
    }
  }
}
