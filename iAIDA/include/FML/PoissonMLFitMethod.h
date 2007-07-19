# ifndef FMLLIB_PoissonMLFitMethod_h__included__
# define FMLLIB_PoissonMLFitMethod_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

# include "FML/FitMethod.h"

namespace fml
{
// namespace FML {

/**
 * Poisson Maximum Likelihood algorithm.
 *
 * See FML::FitMethod for error policy.
 *
 */

class PoissonMLFitMethod : public FitMethodFunction
{
 public:
  
  PoissonMLFitMethod();
  ~PoissonMLFitMethod();

  double value(const std::vector<double>&p) const;

};

// }
}
# endif
