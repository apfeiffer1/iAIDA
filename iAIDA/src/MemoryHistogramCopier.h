// Emacs -*- C++ -*-
#ifndef AIDANATIVE_MEMORYHISTOGRAMCOPIER
#define AIDANATIVE_MEMORYHISTOGRAMCOPIER 1

// includes
#include <string>

// forward declarations
namespace AIDA {
  class IManagedObject;

  namespace Dev {
    class IDevHistogramFactory;
    class IDevManagedObject;
  }
}

namespace iAIDA {
  namespace AIDA_MemoryStore {

/// Class responsible for copying histograms in a memory store

class MemoryHistogramCopier
{
public:
  MemoryHistogramCopier( ); 
  ~MemoryHistogramCopier(){};
  AIDA::Dev::IDevManagedObject* createCopy( const AIDA::IManagedObject& dataObject,
					    AIDA::Dev::IDevHistogramFactory& factory,
					    const std::string& type );
private:

};

  }
}

#endif
