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

#include "MemoryDataPointSetCopier.h"
#include "AIDA_Dev/IDevDataPointSetFactory.h"
#include "AIDA_Dev/IDevDataPointSet.h"
#include <typeinfo>


iAIDA::AIDA_MemoryStore::MemoryDataPointSetCopier::MemoryDataPointSetCopier()
{}


AIDA::Dev::IDevManagedObject*
iAIDA::AIDA_MemoryStore::MemoryDataPointSetCopier::createCopy( const AIDA::IManagedObject& dataObject,  AIDA::Dev::IDevDataPointSetFactory & factory )
{

  try {
    return factory.createCopy( dynamic_cast< const AIDA::IDataPointSet& >( dataObject ) );
  }
  catch( std::bad_cast ) {
  }		      
  return 0;
}
