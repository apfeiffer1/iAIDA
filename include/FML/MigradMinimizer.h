# ifndef FML_MigradMinimizer_h__included__
# define FML_MigradMinimizer_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FmlMinuit
// Author   : Lorenzo Moneta
//
// CERN/PH, Geneva, 2004
//////////////////////////////////////////

#include "MinuitMinimizer.h"

namespace fml { 

/**
 * Minuit minimizers based on Migrad (Variable metric minimizer) 
 *
 * \ingroup FMLMinuit_LIB
 */
  
class MigradMinimizer : public LCGMinuitMinimizer { 

  public : 

    MigradMinimizer() : LCGMinuitMinimizer() {}

   MigradMinimizer(IFMLFunction *objfun) :  LCGMinuitMinimizer(objfun) {}

  // engine creation 

  ROOT::Minuit2::MnApplication * createMinimizer(const ROOT::Minuit2::MnUserParameters & par);  

}; 

}

#endif
