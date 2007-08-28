#ifndef DUMMY_BACKING_STORE
#define DUMMY_BACKING_STORE

#include "IDummyBackingStore.h"
#include "DummyTupleData.h"
#include "DummyPersistentTuple.h"

namespace AIDA {
  namespace Dev {
    class IDevTupleFactory;
  }
}

#include <map>
#include <string>
#include <vector>

class DummyBackingStore : virtual public IDummyBackingStore
{
public:
  DummyBackingStore( AIDA::Dev::IDevTupleFactory& factory );
  ~DummyBackingStore();

  // Methods inherited from the IBackingStore class
  bool writeTupleHeader( AIDA::Dev::ITupleHeader& header );
  bool readTupleHeader( AIDA::Dev::ITupleHeader& header );
  bool bindVariable( AIDA::Dev::ITupleHeader& header, int variableIndex );
  void* variableAddress( AIDA::Dev::ITupleHeader& header, int variableIndex );
  const void* variableAddress( const AIDA::Dev::ITupleHeader& header, int variableIndex ) const;
  void setVariableAddress( const AIDA::Dev::ITupleHeader& header, int variableIndex, void * p );
  bool clearBindings( const AIDA::Dev::ITupleHeader& header );
  bool writeTupleRow( AIDA::Dev::ITupleHeader& header );
  bool readTupleRow( AIDA::Dev::ITupleHeader& header );
  bool resetTuple( AIDA::Dev::ITupleHeader& header );

  // Methods inherited from the IDummyBackingStore class
  bool removePersistentTuple( const std::string& path );
  bool setTupleData( const std::string& path, DummyTupleData* data );
  IDummyPersistentTuple* findPersistentTuple( const std::string& path );

private:
  AIDA::Dev::IDevTupleFactory&                    m_factory;
  std::map< std::string, DummyTupleData* >        m_tupleData;
  std::map< std::string, DummyPersistentTuple >   m_tuples;
};

#endif
