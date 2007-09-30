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

// Header file for class AIDA_FitResult
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Fri Oct  4 14:08:23 2002
// 
// Last update: Fri Oct  4 14:08:23 2002
// 
#ifndef IAIDA_AIDA_FITTER_AIDA_FITRESULT
#define IAIDA_AIDA_FITTER_AIDA_FITRESULT 1

#include "AIDA_Dev/IDevFitResult.h"

# include "AIDA_Function/AIDA_FunctionAdapter.h"
# include "AIDA_FitParameterSettings.h"

# include "FML/FitResult.h"


#include <memory>

namespace iAIDA  { 
  namespace AIDA_Fitter { 


class AIDA_FitResult : virtual public AIDA::Dev::IDevFitResult {

public: 
  AIDA_FitResult(std::auto_ptr<fml::FitResult> r); 
  virtual ~AIDA_FitResult(); 

private:
// usually copying is non trivial, so we make this unaccessible
  AIDA_FitResult(const AIDA_FitResult &); 
  AIDA_FitResult & operator = (const AIDA_FitResult &); 

public: 

      ///////////AIDA user interface methods 

      bool isValid();
      int fitStatus();
      AIDA::IFunction & fittedFunction();
      double quality();
      int ndf();

      const std::vector<double>  & fittedParameters() const;
      std::vector<std::string>   fittedParameterNames() const;
      double fittedParameter(const std::string & name);
      const std::vector<double> & errors() const;
      const std::vector<double> & errorsPlus() const;
      const std::vector<double> & errorsMinus() const;
      double covMatrixElement(int i, int j);
      std::string fitMethodName();
      std::string engineName();
      std::string dataDescription();
      std::vector<std::string>   constraints();
      AIDA::IFitParameterSettings * fitParameterSettings(const std::string& name);
            
      //////////////////////////////
      /// Developer interface methods 
      
      void setIsValid(bool yes);
      void setFitStatus(int status);
      void setFittedFunction(AIDA::IFunction * f);
      void setQuality(double quality);
      void setNdf(int ndf);
      void setCovMatrixElement(int i, int j, double val);
      void setFitMethodName(std::string name);
      void setEngineName(std::string name);
      void setDataDescription(std::string descr);
      void setConstraints(const std::vector<std::string>  & cv);
      void setFitParameterSettings(std::string name, AIDA::IFitParameterSettings * s);

      // fill Function annotation with fit result informations
      void fillAnnotation(AIDA::IFunction * f); 

protected: 

private: 

      std::auto_ptr<fml::FitResult> m_result;
      std::auto_ptr<iAIDA::AIDA_Function::AIDA_FunctionAdapter>  m_fadapter;
      std::vector<std::string> m_constr;

      typedef std::map<std::string, AIDA_FitParameterSettings *> FitParMap;
      mutable FitParMap m_fitpar;

}; 

  }     // end of namespace iAIDA::AIDA_Fitter
}
#endif /* AIDA_FITRESULT */
