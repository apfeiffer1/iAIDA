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

#ifndef MEMORY_BACKING_STORE
#define MEMORY_BACKING_STORE

#include "AIDA_MemoryStore/IMemoryBackingStore.h"
#include "AIDA_MemoryStore/MemoryTupleData.h"
#include "AIDA_MemoryStore/MemoryPersistentTuple.h"

#include <map>
#include <string>
#include <vector>

namespace AIDA {
  namespace Dev {
    class IDevTupleFactory;
  }
}

namespace iAIDA {
  namespace AIDA_MemoryStore {

class MemoryBackingStore : virtual public IMemoryBackingStore
{
public:
  MemoryBackingStore(std::string tuplePluginType = "" );
  ~MemoryBackingStore();

  // Methods inherited from the IBackingStore class
  bool writeTupleHeader( AIDA::Dev::ITupleHeader& header );
  bool readTupleHeader( AIDA::Dev::ITupleHeader& header );
  bool bindVariable( AIDA::Dev::ITupleHeader& header, int variableIndex );
  void* variableAddress( AIDA::Dev::ITupleHeader& header, int variableIndex );
  const void* variableAddress( const AIDA::Dev::ITupleHeader& header, int variableIndex ) const;
  void setVariableAddress( const AIDA::Dev::ITupleHeader& , int , void *  ) { /* not impl.*/ }
  bool clearBindings( const AIDA::Dev::ITupleHeader& header );
  bool writeTupleRow( AIDA::Dev::ITupleHeader& header );
  bool readTupleRow( AIDA::Dev::ITupleHeader& header );
  bool resetTuple( AIDA::Dev::ITupleHeader& header );

  // Methods inherited from the IMemoryBackingStore class
  bool removePersistentTuple( const std::string& path );
  bool setTupleData( const std::string& path, MemoryTupleData* data );
  IMemoryPersistentTuple* findPersistentTuple( const std::string& path );

private:
  AIDA::Dev::IDevTupleFactory *                    m_factory;
  std::map< std::string, MemoryTupleData* >       m_tupleData;
  std::map< std::string, MemoryPersistentTuple >  m_tuples;
};

  }
}
#endif
