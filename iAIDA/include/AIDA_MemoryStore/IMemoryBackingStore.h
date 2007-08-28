#ifndef IMEMORYBACKINGSTORE
#define IMEMORYBACKINGSTORE

#include "AIDA_Dev/IBackingStore.h"

namespace iAIDA {
  namespace AIDA_MemoryStore {

    class IMemoryPersistentTuple;
    class MemoryTupleData;

class IMemoryBackingStore : virtual public AIDA::Dev::IBackingStore
{
public:
  virtual ~IMemoryBackingStore() {}
  virtual bool removePersistentTuple( const std::string& path ) = 0;
  virtual bool setTupleData( const std::string& path, MemoryTupleData* data ) = 0;
  virtual IMemoryPersistentTuple* findPersistentTuple( const std::string& path ) = 0;
};

  }
}
#endif
