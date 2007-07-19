# include "FML/LinearConstraint.h"

namespace fml
{
// namespace FML
// {

/**
 * Create linear constraint.
 * Size of coefficient vector determines the dimensionality of the space.
 */

LinearConstraint::LinearConstraint(std::vector<double> coeff, const Bound& b) : SpaceConstraint(coeff.size(),b), m_coeff(coeff) 
{}

  const std::vector<double>& LinearConstraint::coeff() const { return m_coeff; }

  std::vector<double>& LinearConstraint::coeff() { return m_coeff; }

  LinearConstraint::LinearConstraint(const LinearConstraint& c) : SpaceConstraint(c.dimension(),c.bound())
  {
    //    SpaceConstraint::copyStateFrom(c);
    m_coeff = c.coeff();
  }

  LinearConstraint& LinearConstraint::operator = (const LinearConstraint& c)
  {
    SpaceConstraint::copyStateFrom(c);
    m_coeff = c.coeff();
    return *this;
  }

  bool LinearConstraint::operator ==( const LinearConstraint& c ) const
  {
    return bound() == c.bound() && m_coeff == c.m_coeff;
  }
// }
}
