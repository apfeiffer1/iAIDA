# ifndef FMLLIB_Chi2FitMethod_h__included__
# define FMLLIB_Chi2FitMethod_h__included__

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
 * Typical chi2 algorithm.
 *
 * See FML::FitMethod for error policy.
 *
 * \ingroup FML_LIB
 */

class Chi2FitMethod : public FitMethodFunction
{
 public:
  
  Chi2FitMethod();
  ~Chi2FitMethod();

  double value(const std::vector<double>&) const;

 private:

  Chi2FitMethod(const Chi2FitMethod&) : FitMethodFunction(false,"") {}
  void operator=(const Chi2FitMethod&) {}

  mutable int m_ncall; 
};

// }
}

# endif
