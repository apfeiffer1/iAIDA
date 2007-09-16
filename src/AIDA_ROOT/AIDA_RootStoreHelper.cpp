#include "AIDA/IAxis.h"
#include "AIDA_RootStoreHelper.h"
#
namespace AIDA_RootStore_Helper { 


  /** conversion from Root bin indices to AIDA 
   *
   **/

  int indexAida(int iRoot, int nBin)  {

    if (iRoot == 0) return AIDA::IAxis::UNDERFLOW_BIN; 
    if (iRoot == nBin + 1) return  AIDA::IAxis::OVERFLOW_BIN;
    return iRoot -1; 
  }

  std::string aidaType(const std::string & type) { 
    std::string tmp = ""; 
    if (type == "TH1D" || type == "TH1F") tmp="IHistogram1D"; 
    if (type == "TH2D" || type == "TH2F") tmp="IHistogram2D"; 
    if (type == "TH3D" || type == "TH3F") tmp="IHistogram3D"; 
    if (type == "TProfile" ) tmp="IProfile1D"; 
    if (type == "TProfile2D" ) tmp="IProfile2D"; 
    if (type == "TNtuple" ) tmp="ITuple"; 
    if (type == "TTree" ) tmp="ITuple"; 
    return tmp;
  }

  int getEntries(double binHeight, double binError ) { 
     if (binHeight <=0 ) return 0;
      double xx =  binHeight/binError;
      return int(xx*xx+0.5);
  }


}  
