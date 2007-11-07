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

# ifndef FML_LCGMinuitMinimizer_h__included__
# define FML_LCGLCGMinuitMinimizer_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FmlMinuit
// Author   : Lorenzo Moneta
//
// CERN/PH, Geneva, 2004
//////////////////////////////////////////

# include "FML/MinuitMinimizer.h"


class ROOT::Minuit2::MnApplication; 
class ROOT::Minuit2::MnUserParameters; 


namespace fml {

class IFMLFunction; 
class MinuitFCNWrapper; 

/**
 * Minimizer class for Fumili, specialized Minuit minimizer for least square (chi2) and likelihood methods 
 * 
 *
 * \ingroup FML_LIB
 */

class FumiliMinimizer : public LCGMinuitMinimizer
{
 public:

  /**
   * Default constructor  for the minimizer. 
   * Needs to call later setFunction or reset to set the objective function
   */

  FumiliMinimizer();

  /**
   * Construct the minimizer from the objective function
   * Fumili needs a different FCN than other Minuit Minimizers
   */
  FumiliMinimizer(IFMLFunction *objfun);

  ~FumiliMinimizer() {}



 protected:

  /// create minimization application (engine)
  ROOT::Minuit2::MnApplication * createMinimizer(const ROOT::Minuit2::MnUserParameters & par);  
     
  MinuitFCNWrapper * createFCN( IFMLFunction * f); 
  
};

}


# endif





