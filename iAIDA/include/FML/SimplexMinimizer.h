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

# ifndef FML_SimplexMinimizer_h__included__
# define FML_SimplexMinimizer_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FmlMinuit
// Author   : Lorenzo Moneta
//
// CERN/PH, Geneva, 2004
//////////////////////////////////////////

#include "MinuitMinimizer.h"

namespace fml { 

/**
 * Minuit minimizers based on Simplex 
 *
 * \ingroup FMLMinuit_LIB
 */
  
class SimplexMinimizer : public LCGMinuitMinimizer { 

  public : 

  SimplexMinimizer() : LCGMinuitMinimizer() {}
  
  SimplexMinimizer(IFMLFunction *objfun) :  LCGMinuitMinimizer(objfun) {}

  // engine creation 

  protected : 

  ROOT::Minuit2::MnApplication * createMinimizer(const ROOT::Minuit2::MnUserParameters & par);  


  //const MinimizerControlParams& defaultControlParameters() const; 

}; 

}

#endif
