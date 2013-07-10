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

// Implementation file for class AIDA_FitResult
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Fri Oct  4 14:08:23 2002
// 
// Last update: Fri Oct  4 14:08:23 2002
// 

#include "AIDA_FitResult.h"
#include "FML/Util/debug.h"
#include "FML/Util/util.h"

namespace iAIDA  { 
  namespace AIDA_Fitter { 

    // constructs from an FML Fit Result 
    
    AIDA_FitResult::AIDA_FitResult( std::auto_ptr<fml::FitResult> r) : 
      m_result(r){
      assert(m_result.get());
      // convert FML function to AIDA Function
      std::auto_ptr<fml::IFMLFunction> f(&m_result->fittedFunction() );
      assert (f.get());
      std::auto_ptr<iAIDA::AIDA_Function::AIDA_FunctionAdapter> 
	aida_f(new iAIDA::AIDA_Function::AIDA_FunctionAdapter(f) ); 
      m_fadapter = aida_f; 
      assert(m_fadapter.get()); 
      
      fillAnnotation(m_fadapter.get()); 
    }


    AIDA_FitResult::~AIDA_FitResult() 
    {
    }

    AIDA_FitResult::AIDA_FitResult(const AIDA_FitResult &) 
    {
    }

    AIDA_FitResult & AIDA_FitResult::operator = (const AIDA_FitResult &rhs) 
    {
      if (this == &rhs) return *this;  // time saving self-test
      
      return *this;
    }



    //////////////////////////////
    
    bool AIDA_FitResult::isValid()
    {
      return m_result->isValid();
    }

    int AIDA_FitResult::fitStatus()
    {
      return m_result->fitStatus();
    }
    
    AIDA::IFunction & AIDA_FitResult::fittedFunction()
    {
      return *(m_fadapter.get());
    }
    
    double AIDA_FitResult::quality()
    {
      return m_result->quality();
    }

    int AIDA_FitResult::ndf()
    {
      return m_result->ndf();
    }
    
    const std::vector<double>  & AIDA_FitResult::fittedParameters() const
    { 
      return m_fadapter->parameters(); 
    }
    std::vector<std::string>   AIDA_FitResult::fittedParameterNames() const
    { 
      return m_fadapter->parameterNames(); 
    }
    double AIDA_FitResult::fittedParameter(const std::string & name) { 
      return m_fadapter->parameter(name);
    }



const std::vector<double> & AIDA_FitResult::errors() const 
{ 
  return m_result->errors();
}

const std::vector<double> & AIDA_FitResult::errorsPlus() const
{ 
  return m_result->errorsPlus();
}

const std::vector<double> & AIDA_FitResult::errorsMinus() const
{ 
  return m_result->errorsMinus();
}

double AIDA_FitResult::covMatrixElement(int i, int j)
{
  return m_result->covMatrixElement(i,j);
}
    
std::string AIDA_FitResult::fitMethodName()
{
  return m_result->fitConfig().fitMethodName();
}

std::string AIDA_FitResult::engineName()
{
  return m_result->fitConfig().engineName();
}

std::string AIDA_FitResult::dataDescription()
{
  return m_result->dataDescription();
}

std::vector<std::string>   AIDA_FitResult::constraints()
{
  return m_constr;
}

AIDA::IFitParameterSettings * AIDA_FitResult::fitParameterSettings(const std::string & name)
{
  fml::FitParameterSettings *ps_fml = &m_result->fitConfig().fitParameterSettings(name);
  
  // create a proxy to underlying implementation
  AIDA_FitParameterSettings * ps = new AIDA_FitParameterSettings(ps_fml); 
  m_fitpar[name] = ps;

  return m_fitpar[name];
}
            
//////////////////////////////
      
// I don't have time to implement these setters.
    // They are only required if foreign implementation wanted
    // to set something, but it is not vital for functioning of 
    // FML in the AIDA framework.
    // iAIDA::FMLFitResultSetter should be cleaned and its setter method
    // should be probably not organized in blocks but rather in idividual
    // setters for every thing separately, this would make this wrapper
    // easier to implement.
    // KUBA

void AIDA_FitResult::setIsValid(bool /*yes*/)
{
  INFO_MSG("AIDA_FitResult: setters not implemented");
  /*
    fml::FitResultSetter rs(*m_result.get());
    rs.setFlags(yes, m_result->fitStatus());
  */
}

void AIDA_FitResult::setFitStatus(int /*status*/)
{
  INFO_MSG("AIDA_FitResult: setters not implemented");
  /*
    fml::FitResultSetter rs(*m_result.get());
    rs.setFitStatus(m_result->isValid(), status);
  */
}

void AIDA_FitResult::setFittedFunction(AIDA::IFunction * /*f*/ )
{

  INFO_MSG("AIDA_FitResult: setters not implemented");
  
	/*
	assert(f);

	fml::IFMLFunction *ff = iAIDA::AIDA_Function::getImplementation(f);

	iAIDA::AIDA_Function::FMLFunctionAdapter fa(f);

	if(!ff) 	  
	  ff = &fa;	    
	    
	fml::FitResultSetter rs(*m_result.get());	
	rs.setConfig(m_result->fitConfig(), *ff, m_result->dataDescription());

	m_fadapter = AIDAFunctionAdapter(&m_result.fittedFunction());
	*/
      }

void AIDA_FitResult::setQuality(double /*quality*/ )
      {
	INFO_MSG("AIDA_FitResult: setters not implemented");
      }

void AIDA_FitResult::setNdf(int /*ndf*/ )
      {
	INFO_MSG("AIDA_FitResult: setters not implemented");
      }

      void AIDA_FitResult::setCovMatrixElement(int , int , double )
      {	
	INFO_MSG("AIDA_FitResult: setters not implemented"); 
      }

void AIDA_FitResult::setFitMethodName(std::string /*name*/)
      {
	INFO_MSG("AIDA_FitResult: setters not implemented");
      }
void AIDA_FitResult::setEngineName(std::string /*name*/)
      {
 	INFO_MSG("AIDA_FitResult: setters not implemented");
      }

void AIDA_FitResult::setDataDescription(std::string /*descr*/)
      {
	INFO_MSG("AIDA_FitResult: setters not implemented");
      }
void AIDA_FitResult::setConstraints(const std::vector<std::string>  & /*cv*/)
      {
	INFO_MSG("AIDA_FitResult: setters not implemented");
      }

void AIDA_FitResult::setFitParameterSettings(std::string /*name*/ , AIDA::IFitParameterSettings * /*s*/)
      {
	INFO_MSG("AIDA_FitResult: setters not implemented");
      }
 

      // Utility method
// update annotation with fitted information

void AIDA_FitResult::fillAnnotation(AIDA::IFunction * f) { 
  AIDA::IAnnotation & anno = f->annotation(); 

  std::string s(Util::to_string(quality() ) + " / " + Util::to_string(ndf() ) ); 
  anno.addItem("Fit Method ", fitMethodName() , false);  
  anno.addItem("Fit Quality / ndf ", s , false);  
  std::vector<std::string>  parNames = f->parameterNames(); 
  std::vector<double>  parValues = f->parameters(); 
  for (int i = 0; i < f->numberOfParameters() ; ++ i) { 
    std::string txt = Util::to_string( parValues[i] ) + " +/- " +  Util::to_string( errors()[i] );
    anno.setValue( parNames[i], txt ); 
  }
 
}


  }     // end of namespace iAIDA::AIDA_Fitter
} 
