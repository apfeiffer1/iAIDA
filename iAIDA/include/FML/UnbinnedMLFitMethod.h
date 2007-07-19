# ifndef FMLLIB_UnbinnedMLFitMethod_h__included__
# define FMLLIB_UnbinnedMLFitMethod_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//
//////////////////////////////////////////

// Added for unbinned maximum likelihood fit 
//  Lorenzo Moneta May, 2002

# include "FML/FitMethod.h"

namespace fml {
// namespace FML {

/**
 * Unbinned Maximum Likelihood algorithm.
 *
 * See FML::FitMethod for error policy.
 *
 */

class UnbinnedMLFitMethod : public FitMethodFunction
{
 public:
  
  UnbinnedMLFitMethod();
  ~UnbinnedMLFitMethod();
   
  double value(const std::vector<double>&p) const;

};

// }
}
# endif

