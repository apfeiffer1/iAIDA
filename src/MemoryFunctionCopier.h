// Emacs -*- C++ -*-
#ifndef AIDANATIVE_MEMORYFUNCTIONCOPIER
#define AIDANATIVE_MEMORYFUNCTIONCOPIER 1

// forward declarations
namespace AIDA {
  class IManagedObject;

  namespace Dev {
    class IDevFunctionFactory;
    class IDevManagedObject;
  }
}

namespace iAIDA {
  namespace AIDA_MemoryStore {

/// Class responsible for copying functions in a memory store

class MemoryFunctionCopier
{
public:
  MemoryFunctionCopier(  );
  ~MemoryFunctionCopier(){};
  AIDA::Dev::IDevManagedObject* createCopy( const AIDA::IManagedObject& dataObject,  AIDA::Dev::IDevFunctionFactory& factory );
private:
};

  }
}

#endif
