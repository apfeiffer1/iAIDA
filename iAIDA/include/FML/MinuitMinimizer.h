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

# include "FML/Minimizer.h"
# include "MinuitFCNWrapper.h"

namespace ROOT { 
  namespace Minuit2 { 
    class MnApplication; 
    class MnUserParameters; 
  }
}

namespace fml {

/**
 * Base class for Minuit minimizers.
 *
 * \ingroup FMLMinuit_LIB
 */

class LCGMinuitMinimizer : public Minimizer
{
 public:

  LCGMinuitMinimizer();
  LCGMinuitMinimizer(IFMLFunction *objfun);

  virtual ~LCGMinuitMinimizer();

  /**
   * It is not possible to set constraints in Minuit, so this method always 
   * fails and returns false.
   */
  bool setConstraints(const DomainConstraints&) { return false; }

  virtual const MinimizerControlParams& defaultControlParameters() const;

  bool minimize(); 

 protected:

  // setup minimizer with input parameters
  bool setupMinuit(); 

  // methods that must be overridden by derived classes for creating specialized engines

  virtual ROOT::Minuit2::MnApplication * createMinimizer(const ROOT::Minuit2::MnUserParameters & par /* , strategy */  ) = 0;

  virtual MinuitFCNWrapper * createFCN( IFMLFunction * f) { return new  MinuitFCNWrapper(f); } 

  MinuitFCNWrapper  * getFCN() { return m_fcn; }


 private: 

  /// Auxiliary objective function object interfacing FML Objfunc
  MinuitFCNWrapper  * m_fcn; 
  // minimizer engine 
  ROOT::Minuit2::MnApplication * m_minimizer; 
  
};
}



# endif





