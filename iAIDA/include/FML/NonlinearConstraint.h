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
