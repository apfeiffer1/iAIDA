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
