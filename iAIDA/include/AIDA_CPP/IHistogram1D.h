#ifndef AIDA_CPP_IHISTOGRAM1D_H
#define AIDA_CPP_IHISTOGRAM1D_H


#include "AIDA_Dev/IDevHistogram1D.h"
#include<string>
#include<vector>

namespace AIDA_CPP {

  class IHistogram1D : public AIDA::Dev::IDevHistogram1D {
  public:
    typedef AIDA::IHistogram Base;
    typedef AIDA::IHistogram1D super;
  public:
    virtual void initialize(const std::string & title, int nBins, double lowerEdge, double upperEdge, const std::string & options = "")=0;
    virtual void initialize(const std::string & title, const std::vector<double>  & binEdges, const std::string & options = "")=0;

  };

}
#endif  // AIDA_CPP_IHISTOGRAM_H
