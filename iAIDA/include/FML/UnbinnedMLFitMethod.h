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

# ifndef FMLLIB_UnbinnedMLFitMethod_h__included__
# define FMLLIB_UnbinnedMLFitMethod_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//
//////////////////////////////////////////

// Added for unbinned maximum likelihood fit 
//  Lorenzo Moneta May, 2002

# include "FML/FitMethod.h"

namespace fml {
// namespace FML {

/**
 * Unbinned Maximum Likelihood algorithm.
 *
 * See FML::FitMethod for error policy.
 *
 */

class UnbinnedMLFitMethod : public FitMethodFunction
{
 public:
  
  UnbinnedMLFitMethod();
  ~UnbinnedMLFitMethod();
   
  double value(const std::vector<double>&p) const;

};

// }
}
# endif

