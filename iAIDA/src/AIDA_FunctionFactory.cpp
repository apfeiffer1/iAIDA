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

// Implementation file for class AIDA_FunctionFactory
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Tue Oct  8 16:33:33 2002
// 
// Last update: Tue Oct  8 16:33:33 2002
// 

#include "AIDA_Function/AIDA_FunctionFactory.h"
#include "AIDA_Dev/IDevTree.h"
#include "AIDA_Dev/IDevFunctionFactory.h"
#include "AIDA_Dev/IDevFunction.h"
#include "AIDA_Dev/IDevFunctionCatalog.h"

#include "FML/Util/debug.h" 

namespace iAIDA  { 
  namespace AIDA_Function { 


    AIDA_FunctionFactory::AIDA_FunctionFactory(AIDA::Dev::IDevTree& tree, AIDA::Dev::IDevFunctionFactory& factory) : 
  m_tree( &tree ),
  m_factory( &factory )
{    
}

AIDA_FunctionFactory::~AIDA_FunctionFactory() 
{
}


AIDA_FunctionFactory::AIDA_FunctionFactory(const AIDA_FunctionFactory &) 
{
}


AIDA_FunctionFactory & AIDA_FunctionFactory::operator = (const AIDA_FunctionFactory &rhs) 
{
   if (this == &rhs) return *this;  // time saving self-test

   return *this;
}

// public methods 

AIDA::IFunction * AIDA_FunctionFactory::createFunctionByName(const std::string & name, const std::string & model)
{ 
  AIDA::Dev::IDevFunction * f = m_factory->createFunctionByName(model); 
  
  // LM: temp fixes since codelet does not work for composite functions
  // set model string in codelet to have the function be able to recreate
  if (f) f->setCodeletString(model); 

  return registerFunction(f,name); 
}

AIDA::IFunction * AIDA_FunctionFactory::createFunctionFromScript(const std::string & name, int dim, const std::string & valexpr, const std::string & parameters, const std::string & description, const std::string & gradexpr ) 
{ 
  AIDA::Dev::IDevFunction * f = m_factory->createFunctionFromScript(dim, valexpr, parameters, description, gradexpr); 

  return registerFunction(f,name); 
}

AIDA::IFunction * AIDA_FunctionFactory::cloneFunction(const std::string & name, AIDA::IFunction & f)
{ 
  AIDA::Dev::IDevFunction * f_new = m_factory->cloneFunction(&f);

  return registerFunction(f_new,name); 
}

AIDA::IFunctionCatalog * AIDA_FunctionFactory::catalog()
{
  return m_factory->catalog(); 
}

// private methods 

AIDA::Dev::IDevFunction * AIDA_FunctionFactory::registerFunction(AIDA::Dev::IDevFunction * f, const std::string & name)
{ 

  if (!f) { 
    INFO_MSG("AIDA_FunctionFactory : Error creating function "); 
    return 0; 
  }
  f->setName( name );
  if ( ! m_tree->add( f ) ) {
    delete f; 
    INFO_MSG("AIDA_FunctionFactory : Error adding function to the tree"); 
    return 0; 
  }

  return f;
}

  }     // end of namespace iAIDA::AIDA_Function
} 
