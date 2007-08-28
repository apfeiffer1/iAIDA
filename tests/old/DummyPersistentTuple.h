#ifndef DUMMY_PERSISTENT_TUPLE
#define DUMMY_PERSISTENT_TUPLE

#include <string>
#include <vector>
#include "IDummyPersistentTuple.h"

namespace AIDA {
  namespace Dev {
    class IDevTupleFactory;
    class IDevTuple;
  }
}

class DummyTupleData;
class IDummyBackingStore;

class DummyPersistentTuple : virtual public IDummyPersistentTuple
{
public:
  DummyPersistentTuple( IDummyBackingStore* store,
			AIDA::Dev::IDevTupleFactory& factory,
			DummyTupleData* data,
			const std::string& pathInStore );
  ~DummyPersistentTuple();
  bool setTupleData( DummyTupleData* data );
  DummyTupleData* tupleData() const;
  bool bindVariable( int variableIndex );
  bool clearBindings();
  bool writeTupleRow( int rowNumber );
  bool readTupleRow( int rowNumber );
  void* variableAddress( int variableIndex );
  const void* variableAddress( int variableIndex ) const;

private:
  IDummyBackingStore*           m_store;
  AIDA::Dev::IDevTupleFactory&  m_factory;
  DummyTupleData*               m_tupleData;
  std::string                   m_pathInStore;
  int                           m_currentRow;
  std::vector< void* >          m_cache;
};

#endif
