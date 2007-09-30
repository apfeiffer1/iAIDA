// Emacs -*- C++ -*-
#ifndef IAIDA_MEMORYDATAPOINTSETCOPIER
#define IAIDA_MEMORYDATAPOINTSETCOPIER 1


// forward declarations
namespace AIDA {
  class IManagedObject;

  namespace Dev {
    class IDevDataPointSetFactory;
    class IDevManagedObject;
  }
}

namespace iAIDA {
  namespace AIDA_MemoryStore {

/// Class responsible for copying data point sets in a memory store

class MemoryDataPointSetCopier
{
public:
  MemoryDataPointSetCopier( );
  ~MemoryDataPointSetCopier(){};
  AIDA::Dev::IDevManagedObject* createCopy( const AIDA::IManagedObject& dataObject,   AIDA::Dev::IDevDataPointSetFactory & df );
private:

};

  }
}

#endif
