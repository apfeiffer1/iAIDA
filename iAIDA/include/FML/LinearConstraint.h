# ifndef FML_LinearConstraint_h__included__
# define FML_LinearConstraint_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

# include "FML/SpaceConstraint.h"

# include <vector>

namespace fml
{

// namespace FML {

/**
 * Linear range constraint on the n-dimensional domain space.
 *
 * Linear expression  with given coefficients is used  as a constraint
 * expression.  
 */

class LinearConstraint : public SpaceConstraint
{
 public:

  /// Create linear constraint with given coefficients.
  LinearConstraint(std::vector<double> coeff, const Bound& b );
 
  const std::vector<double>& coeff() const;
  std::vector<double>& coeff();

  LinearConstraint(const LinearConstraint& c);
  LinearConstraint& operator = (const LinearConstraint& c);

  bool operator ==( const LinearConstraint& c ) const;

 private:

  std::vector<double> m_coeff;
};

// }
}

# endif
