#ifndef AIDA_CPP_IPROFILE1D_H
#define AIDA_CPP_IPROFILE1D_H


#include "AIDA_Dev/IDevProfile1D.h"
#include<string>
#include<vector>

namespace AIDA_CPP {

  class IProfile1D : public AIDA::Dev::IDevProfile1D {
  public:
    typedef AIDA::IProfile Base;
    typedef AIDA::IProfile1D super;
  public:
    virtual void initialize(const std::string & title, int nBins, double lowerEdge, double upperEdge, const std::string & options = "")=0;

    virtual void initialize(const std::string & title, const std::vector<double>  & binEdges, const std::string & options = "")=0;

    virtual void initialize(const std::string & title, 
			    int nBins, double lowerEdge, double upperEdge, 
			    double lowerValue, double upperValue, 
			    const std::string & options = "")= 0;

    virtual void initialize(const std::string & title, 
			    const std::vector<double>  & binEdges,
			    double lowerValue, double upperValue, 
			    const std::string & options = "") =0;
  };

}
#endif  // AIDA_CPP_IPROFILE1D_H
