#ifndef AIDA_CPP_IHISTOGRAM3D_H
#define AIDA_CPP_IHISTOGRAM3D_H


#include "AIDA_Dev/IDevHistogram3D.h"
#include<string>
#include<vector>

namespace AIDA_CPP {

  class IHistogram3D : public AIDA::Dev::IDevHistogram3D {
  public:
    typedef AIDA::IHistogram Base;
    typedef AIDA::IHistogram3D super;
  public:
    virtual void initialize(const std::string & title, int binsX, double iminX, double imaxX, int binsY, double iminY, double imaxY, int binsZ, double iminZ, double imaxZ, const std::string & options="") = 0;

    virtual void initialize(const std::string & title, const std::vector<double>  & binEdgesX, const std::vector<double>  & binEdgesY, const std::vector<double>  & binEdgesZ, const std::string & options="") = 0;

  };

}
#endif  // AIDA_CPP_IHISTOGRAM_H
