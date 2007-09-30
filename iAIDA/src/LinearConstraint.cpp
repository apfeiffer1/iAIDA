//
//    Copyright 2007 Andreas Pfeiffer
//
/*
 *  This file is part of iAIDA.
 *
 *  iAIDA is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as 
 *  published by the Free Software Foundation, either version 3 of 
 *  the License.
 *
 *  iAIDA is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public 
 *  License along with iAIDA.  If not, see <http://www.gnu.org/licenses/>.
 */

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
