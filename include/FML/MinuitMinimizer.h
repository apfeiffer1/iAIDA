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

# include "FML/Minimizer.h"
# include "MinuitFCNWrapper.h"

namespace ROOT { 
  namespace Minuit2 { 
    class MnApplication; 
    class MnUserParameters; 
  }
}

namespace fml {

/**
 * Base class for Minuit minimizers.
 *
 * \ingroup FMLMinuit_LIB
 */

class LCGMinuitMinimizer : public Minimizer
{
 public:

  LCGMinuitMinimizer();
  LCGMinuitMinimizer(IFMLFunction *objfun);

  virtual ~LCGMinuitMinimizer();

  /**
   * It is not possible to set constraints in Minuit, so this method always 
   * fails and returns false.
   */
  bool setConstraints(const DomainConstraints&) { return false; }

  virtual const MinimizerControlParams& defaultControlParameters() const;

  bool minimize(); 

 protected:

  // setup minimizer with input parameters
  bool setupMinuit(); 

  // methods that must be overridden by derived classes for creating specialized engines

  virtual ROOT::Minuit2::MnApplication * createMinimizer(const ROOT::Minuit2::MnUserParameters & par /* , strategy */  ) = 0;

  virtual MinuitFCNWrapper * createFCN( IFMLFunction * f) { return new  MinuitFCNWrapper(f); } 

  MinuitFCNWrapper  * getFCN() { return m_fcn; }


 private: 

  /// Auxiliary objective function object interfacing FML Objfunc
  MinuitFCNWrapper  * m_fcn; 
  // minimizer engine 
  ROOT::Minuit2::MnApplication * m_minimizer; 
  
};
}



# endif





