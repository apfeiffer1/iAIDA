# ifndef FML_LCGMinuitMinimizer_h__included__
# define FML_LCGLCGMinuitMinimizer_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FmlMinuit
// Author   : Lorenzo Moneta
//
// CERN/PH, Geneva, 2004
//////////////////////////////////////////

# include "FML/MinuitMinimizer.h"


class ROOT::Minuit2::MnApplication; 
class ROOT::Minuit2::MnUserParameters; 


namespace fml {

class IFMLFunction; 
class MinuitFCNWrapper; 

/**
 * Minimizer class for Fumili, specialized Minuit minimizer for least square (chi2) and likelihood methods 
 * 
 *
 * \ingroup FML_LIB
 */

class FumiliMinimizer : public LCGMinuitMinimizer
{
 public:

  /**
   * Default constructor  for the minimizer. 
   * Needs to call later setFunction or reset to set the objective function
   */

  FumiliMinimizer();

  /**
   * Construct the minimizer from the objective function
   * Fumili needs a different FCN than other Minuit Minimizers
   */
  FumiliMinimizer(IFMLFunction *objfun);

  ~FumiliMinimizer() {}



 protected:

  /// create minimization application (engine)
  ROOT::Minuit2::MnApplication * createMinimizer(const ROOT::Minuit2::MnUserParameters & par);  
     
  MinuitFCNWrapper * createFCN( IFMLFunction * f); 
  
};

}


# endif





