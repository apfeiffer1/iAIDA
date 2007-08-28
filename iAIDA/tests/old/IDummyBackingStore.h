#ifndef IDUMMYBACKINGSTORE
#define IDUMMYBACKINGSTORE

#include "AIDA_Dev/IBackingStore.h"

class IDummyPersistentTuple;
class DummyTupleData;

class IDummyBackingStore : virtual public AIDA::Dev::IBackingStore
{
public:
  virtual ~IDummyBackingStore() {}
  virtual bool removePersistentTuple( const std::string& path ) = 0;
  virtual bool setTupleData( const std::string& path, DummyTupleData* data ) = 0;
  virtual IDummyPersistentTuple* findPersistentTuple( const std::string& path ) = 0;
};

#endif
