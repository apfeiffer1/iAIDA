// Emacs -*- C++ -*-
#ifndef IAIDA_ROOTSTOREFACTORY
#define IAIDA_ROOTSTOREFACTORY 1

#include <string>
#include "AIDA_Dev/IStoreFactory.h"

namespace iAIDA {
  namespace AIDA_RootStore {

/// Implementation of AIDA::DEV::IStoreFactory interface for a factory of Root stores

class AIDA_RootStoreFactory : virtual public AIDA::Dev::IStoreFactory
{
public:
  /// Constructor
  AIDA_RootStoreFactory();
  /// Destructor
  ~AIDA_RootStoreFactory();

  /// Method inherited from the IStoreFactory class
  AIDA::Dev::IStore* createStore( const std::string & storeName,
				  bool readOnly,
				  bool createNew,
				  const std::string & options );
  bool destroyStore( AIDA::Dev::IStore* store );
};

  }
}

#endif /* ifndef IAIDA_RootSTOREFACTORY */
