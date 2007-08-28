# ifndef FML_SimplexMinimizer_h__included__
# define FML_SimplexMinimizer_h__included__

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
 * Minuit minimizers based on Simplex 
 *
 * \ingroup FMLMinuit_LIB
 */
  
class SimplexMinimizer : public LCGMinuitMinimizer { 

  public : 

  SimplexMinimizer() : LCGMinuitMinimizer() {}
  
  SimplexMinimizer(IFMLFunction *objfun) :  LCGMinuitMinimizer(objfun) {}

  // engine creation 

  protected : 

  ROOT::Minuit2::MnApplication * createMinimizer(const ROOT::Minuit2::MnUserParameters & par);  


  //const MinimizerControlParams& defaultControlParameters() const; 

}; 

}

#endif
