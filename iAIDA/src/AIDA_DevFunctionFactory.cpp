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

// Implementation file for class AIDA_DevFunctionFactory
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Tue Oct  8 16:33:42 2002
// 
// Last update: Tue Oct  8 16:33:42 2002
// 

#include "AIDA_Function/AIDA_DevFunctionFactory.h"
#include "AIDA_Dev/IDevFunction.h"
#include "AIDA_Dev/IDevFunctionCatalog.h"
#include "AIDA_Function/AIDA_FunctionCatalog.h"
#include "AIDA_Function/AIDA_FunctionAdapter.h"

#include "FML/CodeletStringParser.h"

#include "FML/Util/debug.h"


namespace iAIDA  { 
  namespace AIDA_Function { 

    // location of function catalog
    //static const std::string repository_dir = ""; 
    //static const std::string dbfile         = ""; 

AIDA_DevFunctionFactory::AIDA_DevFunctionFactory() 
{
  // create catalog 
  // catalog is returned by pointer - so I cannot destroy in destructors- 
  // who is going to destroy it ? the user ?  
  //m_cat = new AIDA_FunctionCatalog(repository_dir,dbfile); 
   m_cat = new AIDA_FunctionCatalog(); 
}

AIDA_DevFunctionFactory::~AIDA_DevFunctionFactory() 
{
}

AIDA_DevFunctionFactory::AIDA_DevFunctionFactory(const AIDA_DevFunctionFactory &) 
{
}

AIDA_DevFunctionFactory & AIDA_DevFunctionFactory::operator = (const AIDA_DevFunctionFactory &rhs) 
{
   if (this == &rhs) return *this;  // time saving self-test

   return *this;
}

// public methods 

AIDA::Dev::IDevFunction * AIDA_DevFunctionFactory::createFunctionByName(std::string model) 
{ 
  // check if is a codelet (should move in a different method !!) 
  /* 
  fml::CodeletStringParser parser; 
  std::string codelet = model; 
  if (parser.read(codelet))
    model = parser.typenameId;
  */ 
  
  AIDA::IFunction *f =  m_cat->create(model); 
  // need to down cast
  AIDA::Dev::IDevFunction * df = dynamic_cast<AIDA::Dev::IDevFunction *>(f); 
  return df; 
} 


AIDA::Dev::IDevFunction * AIDA_DevFunctionFactory::createFunctionFromScript(int /* dim */, std::string /* valexpr */ , std::string /* parameters */ , std::string /* description */ , std::string /* gradexpr */ ) 
{ 
  INFO_MSG("iAIDA::AIDA_Function::AIDA_DevFunctionFactory - sorry, createFunctionFromScript not yet implemented ! "); 
  return 0; 
}

AIDA::Dev::IDevFunction * AIDA_DevFunctionFactory::cloneFunction(AIDA::IFunction * f)
{ 

  AIDA_FunctionAdapter* af = dynamic_cast<AIDA_FunctionAdapter*>(f);
  if(!af)
    {
      INFO_MSG("AIDA_Fitter_FML::AIDA_DevFunctionFactory: sorry, can clone only AIDA_Function_FML functions, you requested a clone of a foreign implementation");
      return 0;
    }
	
  AIDA::Dev::IDevFunction *nf = new AIDA_FunctionAdapter( af->getImplementation()->clone() );

  return nf;

}

AIDA::Dev::IDevFunctionCatalog * AIDA_DevFunctionFactory::catalog()
{
  return m_cat; 
}


  }     // end of namespace iAIDA::AIDA_Function
} 
