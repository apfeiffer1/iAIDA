# ifndef FML_DefaultMinuitMinimizer_h__included__
# define FML_DefaultMinuitMinimizer_h__included__

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
 * Default Minuit minimizer which is a combination of Migrad and Simplex (MnMinimize) 
 *
 * \ingroup FMLMinuit_LIB
 */
  
class DefaultMinuitMinimizer : public LCGMinuitMinimizer { 

 public : 
    
  DefaultMinuitMinimizer() : LCGMinuitMinimizer() {}

  DefaultMinuitMinimizer(IFMLFunction *objfun) :  LCGMinuitMinimizer(objfun) {}

  // engine creation 

  ROOT::Minuit2::MnApplication * createMinimizer(const ROOT::Minuit2::MnUserParameters & par);  

}; 

}

#endif
