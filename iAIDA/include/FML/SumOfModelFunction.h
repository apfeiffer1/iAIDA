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

# ifndef FMLLIB__SumOfModelFunction_h__included__
# define FMLLIB__SumOfModelFunction_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

# include "FML/CompoundModelFunction.h"

namespace fml
{

// namespace FML
// {

class SumOfModelFunction : public CompoundModelFunction
{
public:

  SumOfModelFunction(bool members_owned);

  /// Constructing the function name "on the fly"...
  bool append(IFMLFunction *member_fun);

  void appendAmpParameters();

  double value(const std::vector<double>& x) const;
  const std::vector<double>& gradient(const std::vector<double>& x)  const;
  const std::vector<double>& parameterGradient(const std::vector<double>& x) const;
  void normalize(bool on); 


 protected: 
 private: 

  bool m_first; 
  int m_iAmpl; 
}; 

// }

}

# endif
