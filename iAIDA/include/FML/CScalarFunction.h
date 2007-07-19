# ifndef FMLLIB_CScalarFunction_h__included__
# define FMLLIB_CScalarFunction_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

# include "FML/Function.h"

namespace fml {
// namespace FML {

/** 
 * Scalar function implemented with a global function pointer.
 *
 * This is a wrapper for global functions.
 *
 * \ingroup FML_LIB
 */

class CScalarFunction : public Function
{
public:

  /// Signature of global function that may be captured.
  typedef double (*cscalar_fptr)(const vector<double>& d);
  
  /// Capture global function and specify dimensionality.
  CScalarFunction(cscalar_fptr fptr, int dim) : Function(dim,0,false,false,"cscalar_"), m_fptr(fptr) {}

  /// Global function call to compute function value.
  double value(const vector<double>& d) const 
  {
    return (*m_fptr)(d);
  }

  /// Check for equality of two functions.
  /// CScalarFunction objects are assumed equal if they captured the same
  /// function (i.e. if pointers to global function are equal).
  /// It is not possible nor feasible to check if two different
  /// global functions give the same results (are mathematicaly equal).
  bool isEqual(const IFMLFunction * f) const
  {

    /// PENDING: dynamic cast does not work on Solaris

# ifdef FML_SOLARIS_COMPILER
    return false;
# else
    const CScalarFunction *cf = dynamic_cast<const CScalarFunction*>(f);
    return cf ? (m_fptr == cf->m_fptr) : false;
# endif
  }

  std::auto_ptr<IFMLFunction> clone() const
  {
    return std::auto_ptr<IFMLFunction>(new CScalarFunction(m_fptr,dimension()));
  }

private:

  cscalar_fptr m_fptr;
};

// }
}
# endif
