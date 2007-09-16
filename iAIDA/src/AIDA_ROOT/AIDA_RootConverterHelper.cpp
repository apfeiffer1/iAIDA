#include "AIDA/IAxis.h"
#include "AIDA_RootConverterHelper.h"
#
namespace AIDA_RootConverter_Helper { 


  /** conversion from Root bin indices to AIDA 
   *
   **/

  int indexAida(int iRoot, int nBin)  {

    if (iRoot == 0) return AIDA::IAxis::UNDERFLOW_BIN; 
    if (iRoot == nBin + 1) return  AIDA::IAxis::OVERFLOW_BIN;
    return iRoot -1; 
  }

  int getEntries(double binHeight, double binError ) { 
     if (binHeight <=0 ) return 0;
      double xx =  binHeight/binError;
      return int(xx*xx+0.5);
  }

}  
