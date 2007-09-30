// Emacs -*- C++ -*-
#ifndef IAIDA_MEMORYSTOREFACTORY
#define IAIDA_MEMORYSTOREFACTORY 1

#include <string>
#include "AIDA_Dev/IStoreFactory.h"

namespace iAIDA {
  namespace AIDA_MemoryStore {

/// Implementation of AIDA::DEV::IStoreFactory interface for a factory of memory stores

class AIDA_MemoryStoreFactory : virtual public AIDA::Dev::IStoreFactory
{
public:
  /// Constructor
  AIDA_MemoryStoreFactory();
  /// Destructor
  ~AIDA_MemoryStoreFactory();

  /// Method inherited from the IStoreFactory class
  AIDA::Dev::IStore* createStore( const std::string & storeName,
				  bool readOnly,
				  bool createNew,
				  const std::string & options );
  bool destroyStore( AIDA::Dev::IStore* store );
};

  }
}

#endif /* ifndef IAIDA_MEMORYSTOREFACTORY */
