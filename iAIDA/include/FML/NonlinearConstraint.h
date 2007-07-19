# ifndef FML_NonlinearConstraint_h__included__
# define FML_NonlinearConstraint_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

# include "FML/IFMLFunction.h"

# include "FML/SpaceConstraint.h"

namespace fml
{

// namespace FML {

/**
 * Nonlinear range constraint on the n-dimensional domain space.
 *
 * Arbitrary function may be used as a constraint expression (it is called
 * constraint function), which is stored externally and referenced by
 * pointer.
 *
 * If NonlinearConstraint objects copied by value, both copies reference the
 * same function.
 */

class NonlinearConstraint : public SpaceConstraint
{
 public:

  /// Create non-linear constraint with given function.
  NonlinearConstraint(IFMLFunction &f, const Bound &b);

  IFMLFunction &function() const;

  NonlinearConstraint(const NonlinearConstraint& c);
  NonlinearConstraint& operator = (const NonlinearConstraint& c);
  bool operator == (const NonlinearConstraint& c ) const;

 private:

  std::auto_ptr<IFMLFunction> m_fun;
};

// }
}
# endif
