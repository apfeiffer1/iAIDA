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
