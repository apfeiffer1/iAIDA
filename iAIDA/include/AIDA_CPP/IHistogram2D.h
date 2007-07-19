#ifndef AIDA_CPP_IHISTOGRAM2D_H
#define AIDA_CPP_IHISTOGRAM2D_H


#include "AIDA_Dev/IDevHistogram2D.h"
#include<string>
#include<vector>

namespace AIDA_CPP {

  class IHistogram2D : public AIDA::Dev::IDevHistogram2D {
  public:
    typedef AIDA::IHistogram Base;
    typedef AIDA::IHistogram2D super;
  public:
    virtual void initialize(const std::string & title, int binsX, double iminX, double imaxX, int binsY, double iminY, double imaxY, const std::string & options="") = 0;

    virtual void initialize(const std::string & title, const std::vector<double>  & binEdgesX, const std::vector<double>  & binEdgesY, const std::string & options="") = 0;

  };

}
#endif  // AIDA_CPP_IHISTOGRAM_H
