#include "TMath.h"
#include "TRandom3.h"

#ifndef __GCCXML__

inline std::string itoa(int i) 
{
  char res[10];
  sprintf(res, "%d", i);
  std::string ret(res);
  return ret;
}

inline std::string ftoa(double i) 
{
  char res[10];
  sprintf(res, "%f", i);
  std::string ret(res);
  return ret;
}

static TRandom3 *r3=new TRandom3();

inline void removeSpaces(std::string &input)
{
  input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
}

#endif
