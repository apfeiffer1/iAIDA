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

# include "FML/NonlinearConstraint.h"

namespace fml
{
// namespace FML
// {
  /// Create non-linear constraint with given function.
  NonlinearConstraint::NonlinearConstraint(IFMLFunction &f, const Bound &b) : 
    SpaceConstraint(f.dimension(),b), m_fun(f.clone()) {}

  IFMLFunction &NonlinearConstraint::function() const 
  {
    assert(m_fun.get());
    return *m_fun.get(); 
  }

  NonlinearConstraint::NonlinearConstraint(const NonlinearConstraint& c) : SpaceConstraint(c.dimension(), c.bound()), m_fun(0)
  {
    //    SpaceConstraint::copyStateFrom(c);
    std::auto_ptr<IFMLFunction> nf(c.function().clone());
    m_fun = nf;
  }

  NonlinearConstraint& NonlinearConstraint::operator = (const NonlinearConstraint& c)
  {
    SpaceConstraint::copyStateFrom(c);
    std::auto_ptr<IFMLFunction> nf(c.function().clone());
    m_fun = nf;
    return *this;
  }

  bool NonlinearConstraint::operator == (const NonlinearConstraint& c ) const
  {
    return bound()==c.bound() && function().isEqual(&c.function());
  }
// }
}
