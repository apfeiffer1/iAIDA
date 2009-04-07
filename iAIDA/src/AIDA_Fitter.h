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

 /**********************************************************************
  *                                                                    *
  * Copyright (c) 2002 Lorenzo Moneta, CERN/IT                       *
  *                   <Lorenzo.Moneta.cern.ch>                       *
  *                                                                    *
  * This library is free software; you can redistribute it and/or      *
  * modify it under the terms of the GNU Lesser General Public         *
  * License as published by the Free Software Foundation; either       *
  * version 2.1 of the License, or (at your option) any later version. *
  *                                                                    *
  * This library is distributed in the hope that it will be useful,    *
  * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   *
  * Lesser General Public License for more details.                    *
  *                                                                    *
  * You should have received a copy of the GNU Lesser General Public   *
  * License along with this library (see file COPYING); if not, write  *
  * to the Free Software Foundation, Inc., 59 Temple Place, Suite      *
  * 330, Boston, MA 02111-1307 USA, or contact the author.             *
  *                                                                    *
  **********************************************************************/

// Header file for class AIDA_Fitter
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Thu Oct  3 15:00:32 2002
// 
// Last update: Thu Oct  3 15:00:32 2002
// 
#ifndef IAIDA_AIDA_FITTER_AIDA_FITTER
#define IAIDA_AIDA_FITTER_AIDA_FITTER 1

#include "AIDA_Dev/IDevFitter.h"

#include <vector>
#include <map>


namespace fml { 
     class IFMLFunction; 
     class Fitter; 
 }

namespace iAIDA  { 
  namespace AIDA_Fitter { 

    class AIDA_FitParameterSettings; 


class AIDA_Fitter : virtual public AIDA::Dev::IDevFitter {

public: 
  AIDA_Fitter(); 
  virtual ~AIDA_Fitter(); 

private:
// usually copying is non trivial, so we make this unaccessible
  AIDA_Fitter(const AIDA_Fitter &); 
  AIDA_Fitter & operator = (const AIDA_Fitter &); 

public: 

  /// methods from AIDA::IFitter

    /**
     * Fit an IFunction to an IFitData.
     */
  AIDA::IFitResult * fit(AIDA::IFitData & fitData, AIDA::IFunction & func) ;

    /**
     * Shortcut to fit an IBaseHistogram directly with an IFunction. 
     */
  AIDA::IFitResult * fit(AIDA::IBaseHistogram & hist, AIDA::IFunction & func) ;

    /**
     * Shortcut to fit an IBaseHistogram directly with an IFunction. 
     */
  AIDA::IFitResult * fit(AIDA::IBaseHistogram & hist, const std::string& model) ;

    /**
     * Shortcut to fit an IBaseHistogram directly with an IFunction. 
     */
  AIDA::IFitResult * fit(AIDA::IBaseHistogram & hist, const std::string& model, const std::vector<double>  & initialParameters) ;

    /**
     * Shortcut to fit an IDataPointSet directly with an IFunction. 
     */
  AIDA::IFitResult * fit(AIDA::IDataPointSet & dataPointSet, AIDA::IFunction & f) ;

    /**
     * Shortcut to fit an IDataPointSet directly with an IFunction. 
     */
  AIDA::IFitResult * fit(AIDA::IDataPointSet & dataPointSet, const std::string& model) ;

    /**
     * Shortcut to fit an IDataPointSet directly with an IFunction. 
     */
  AIDA::IFitResult * fit(AIDA::IDataPointSet & dataPointSet, const std::string& model, const std::vector<double>  & initialParameters) ;

  /// Shortcut for the predefined models.
  AIDA::IFitResult * fit(AIDA::IFitData & d, const std::string& model) ;

  AIDA::IFitResult * fit(AIDA::IFitData & d, const std::string& model, const std::vector<double>  & initialParameters) ;

    /**
     * Set the optimization engine which is used internally for fitting.
     */
  bool setEngine(const std::string& name) ;

    /**
     * Get the name of the engine currently in use.
     */
  std::string engineName() ;

    /**
     * Access to fit parameter settings. FitParameterSettings object belongs to
     */
  AIDA::IFitParameterSettings & fitParameterSettings(const std::string& name) ;

    /**
     * Get a vector of names of all parameter settings defined in the fitter.
     */
  std::vector<std::string>   listParameterSettings() ;

    /**
     * Reset all parameter settings which fitter remebers.
     */
  void resetParameterSettings() ;

    /**
     * Set a general constraint in the parameter space. The constraint
     * is a parsed expression. PENDING: The syntax to be fixed.
     */
  bool setConstraint(const std::string& expr) ;

    /// Get the list of all constraints.
  std::vector<std::string>   constraints() ;

    /**
     * Reset all the constraints.
     */
  void resetConstraints() ;

    /**
     * Set the fit method type. 
     */
  bool setFitMethod(const std::string& name);

    /**
     * Get the name of the fit method currently in use.
     */
  std::string fitMethodName() ;

    /// Create a 1D scan of the fit quality for given parameter 'par' of
    /// the function.
  AIDA::IDataPointSet * createScan1D(AIDA::IFitData & d, AIDA::IFunction & f, const std::string& par, int npts, double pmin, double pmax) ;

    /// Create a 2D contour spanned on 'par1' and 'par2',
    /// with given number of npts points. Contour is create using previously
    /// obtained fit results. Argument 'up' tells the vertical distance of
    /// the contour plane above the minimal fit quality value.
  AIDA::IDataPointSet * createContour(AIDA::IFitData & d, AIDA::IFitResult & r, const std::string& par1, const std::string& par2, int npts, double up) ;

    /**
     * Choose between using or not using the
     * analytical derivatives if provided by the function.
     * @param useGrad If <code>true</code> the IFitter will use the
     *                analytical derivatives if provided by the IFunction.
     *
     */
  void setUseFunctionGradient(bool useGrad) ;

    /**
     * Check if the IFitter is set to use the analytical derivatives provided
     * by the IFunction.
     * @return <code>true</code> if the IFitter uses the analytical derivatives.
     *
     */
  bool useFunctionGradient() ;


  ///  not in AIDA interface
  // need to be outside constructors because some options must be set 
  // after having choosen the fit type(e.g. errorUp)
  bool setOptions(const std::string & options); 

  

protected: 

  AIDA::IFitResult * fit(AIDA::IFitData& d, fml::IFMLFunction &f);
  
  bool connect(AIDA::IFitData& d, AIDA::IBaseHistogram &h); 
  bool connect(AIDA::IFitData& d, AIDA::IDataPointSet &dps); 


private: 

  fml::Fitter * m_fitter; 

  typedef std::map<std::string, AIDA_FitParameterSettings *> FitParMap;
  FitParMap m_parmap;
  std::vector<std::string> m_constr;


}; 

  }
}
#endif /* AIDA_FITTER */
