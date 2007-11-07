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

#ifndef MEMORY_PERSISTENT_TUPLE
#define MEMORY_PERSISTENT_TUPLE

#include <string>
#include <vector>
#include "IMemoryPersistentTuple.h"

namespace AIDA {
  namespace Dev {
    class IDevTupleFactory;
  }
}

namespace iAIDA {
  namespace AIDA_MemoryStore {

    class MemoryTupleData;
    class IMemoryBackingStore;

class MemoryPersistentTuple : virtual public IMemoryPersistentTuple
{
public:
  MemoryPersistentTuple( IMemoryBackingStore* store,
			 AIDA::Dev::IDevTupleFactory& factory,
			 MemoryTupleData* data,
			 const std::string& pathInStore );
  ~MemoryPersistentTuple();
  bool setTupleData( MemoryTupleData* data );
  MemoryTupleData* tupleData() const;
  bool bindVariable( int variableIndex );
  bool clearBindings();
  bool writeTupleRow( int rowNumber );
  bool readTupleRow( int rowNumber );
  void* variableAddress( int variableIndex );
  const void* variableAddress( int variableIndex ) const;

private:
  IMemoryBackingStore*          m_store;
  AIDA::Dev::IDevTupleFactory&  m_factory;
  MemoryTupleData*              m_tupleData;
  std::string                   m_pathInStore;
  int                           m_currentRow;
  std::vector< void* >          m_cache;
};

  }
}
#endif
