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
