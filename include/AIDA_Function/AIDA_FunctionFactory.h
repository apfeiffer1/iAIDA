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

// Header file for class AIDA_FunctionFactory
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Tue Oct  8 16:33:33 2002
// 
// Last update: Tue Oct  8 16:33:33 2002
// 
#ifndef AIDANATIVE_AIDA_FUNCTION_AIDA_FUNCTIONFACTORY
#define AIDANATIVE_AIDA_FUNCTION_AIDA_FUNCTIONFACTORY 1

#include "AIDA/IFunctionFactory.h"

namespace AIDA {
  namespace Dev {
    class IDevTree;
    class IDevFunctionFactory;
    class IDevFunction; 
  }
}

namespace iAIDA  { 
  namespace AIDA_Function { 


class AIDA_FunctionFactory : virtual public AIDA::IFunctionFactory {

public: 
  AIDA_FunctionFactory( AIDA::Dev::IDevTree& tree, AIDA::Dev::IDevFunctionFactory& factory); 
  virtual ~AIDA_FunctionFactory(); 

private:
// usually copying is non trivial, so we make this unaccessible
  AIDA_FunctionFactory(const AIDA_FunctionFactory &); 
  AIDA_FunctionFactory & operator = (const AIDA_FunctionFactory &); 

public: 

  // create functions (from AIDA interface) 
  AIDA::IFunction * createFunctionByName(const std::string & name, const std::string & model); 

  AIDA::IFunction * createFunctionFromScript(const std::string & name, int dim, const std::string & valexpr, const std::string & parameters, const std::string & description, const std::string & gradexpr = ""); 

  AIDA::IFunction * cloneFunction(const std::string & name, AIDA::IFunction & f);

  //get access to the function catalog
  AIDA::IFunctionCatalog * catalog();

protected: 

  AIDA::Dev::IDevFunction * registerFunction(AIDA::Dev::IDevFunction * f, const std::string & name);  


private: 

  // data members : references to tree and dev factory
  AIDA::Dev::IDevTree * m_tree; 
  AIDA::Dev::IDevFunctionFactory * m_factory; 


}; 

  }     // end of namespace iAIDA::AIDA_Function
}
#endif /* AIDA_FUNCTIONFACTORY */
