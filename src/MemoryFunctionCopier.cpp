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

#include "MemoryFunctionCopier.h"
#include "AIDA_Dev/IDevFunctionFactory.h"
#include "AIDA_Dev/IDevFunction.h"
#include <typeinfo>


iAIDA::AIDA_MemoryStore::MemoryFunctionCopier::MemoryFunctionCopier(  ){}


AIDA::Dev::IDevManagedObject*
iAIDA::AIDA_MemoryStore::MemoryFunctionCopier::createCopy( const AIDA::IManagedObject& dataObject, AIDA::Dev::IDevFunctionFactory& factory )
{

  try {
    const AIDA::IFunction& function = dynamic_cast< const AIDA::IFunction& >( dataObject );
    return factory.cloneFunction( &( const_cast< AIDA::IFunction& >( function ) ) );
  }
  catch( std::bad_cast ) {
  }		      
  return 0;
}
