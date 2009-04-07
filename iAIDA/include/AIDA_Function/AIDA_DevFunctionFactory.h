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

// Header file for class AIDA_DevFunctionFactory
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Tue Oct  8 16:33:42 2002
// 
// Last update: Tue Oct  8 16:33:42 2002
// 
#ifndef IAIDA_AIDA_FUNCTION_AIDA_DEVFUNCTIONFACTORY
#define IAIDA_AIDA_FUNCTION_AIDA_DEVFUNCTIONFACTORY 1

#include "AIDA_Dev/IDevFunctionFactory.h"

namespace AIDA {
   class IFunction; 
   namespace Dev {
     class IDevFunctionCatalog;
     class IDevFunctionFactory;
     class IDevFunction; 
   }
}


namespace iAIDA  { 
  namespace AIDA_Function { 


class AIDA_DevFunctionFactory : virtual public AIDA::Dev::IDevFunctionFactory {

public: 
  AIDA_DevFunctionFactory(); 
  virtual ~AIDA_DevFunctionFactory(); 

private:
// usually copying is non trivial, so we make this unaccessible
  AIDA_DevFunctionFactory(const AIDA_DevFunctionFactory &); 
  AIDA_DevFunctionFactory & operator = (const AIDA_DevFunctionFactory &); 

public: 

  AIDA::Dev::IDevFunction * createFunctionByName(std::string model);

 AIDA::Dev::IDevFunction * createFunctionFromScript(int dim, std::string valexpr, std::string parameters, std::string description, std::string gradexpr = ""); 
 AIDA::Dev::IDevFunction * cloneFunction(AIDA::IFunction * f);

 AIDA::Dev::IDevFunctionCatalog * catalog();

protected: 


private: 

 AIDA::Dev::IDevFunctionCatalog*  m_cat; 

}; 

  }     // end of namespace iAIDA::AIDA_Function
}
#endif /* AIDA_DEVFUNCTIONFACTORY */
