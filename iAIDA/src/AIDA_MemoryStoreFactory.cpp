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

#include "AIDA_MemoryStoreFactory.h"
#include "AIDA_MemoryStore.h"


iAIDA::AIDA_MemoryStore::AIDA_MemoryStoreFactory::AIDA_MemoryStoreFactory()
{}

iAIDA::AIDA_MemoryStore::AIDA_MemoryStoreFactory::~AIDA_MemoryStoreFactory()
{}


AIDA::Dev::IStore*
iAIDA::AIDA_MemoryStore::AIDA_MemoryStoreFactory::createStore( const std::string & storeName,
								bool,
								bool,
								const std::string & )
{
  return new iAIDA::AIDA_MemoryStore::AIDA_StoreMemory( storeName );
}


bool
iAIDA::AIDA_MemoryStore::AIDA_MemoryStoreFactory::destroyStore( AIDA::Dev::IStore* store )
{
  if ( store ) {
    delete store;
    return true;
  }
  else {
    return false;
  }
}

