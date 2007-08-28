// Emacs -*- C++ -*-
#ifndef AIDANATIVE_XMLSTOREFACTORY
#define AIDANATIVE_XMLSTOREFACTORY 1

#include <string>
#include "AIDA_Dev/IStoreFactory.h"

namespace iAIDA {
  namespace AIDA_XMLStore {

/// Implementation of AIDA::DEV::IStoreFactory interface for a factory of XML stores

class AIDA_XMLStoreFactory : virtual public AIDA::Dev::IStoreFactory
{
public:
  /// Constructor
  AIDA_XMLStoreFactory();
  /// Destructor
  ~AIDA_XMLStoreFactory();

  /// Method inherited from the IStoreFactory class
  AIDA::Dev::IStore* createStore( const std::string & storeName,
				  bool readOnly,
				  bool createNew,
				  const std::string & options );
  bool destroyStore( AIDA::Dev::IStore* store );
};

  }
}

#endif /* ifndef AIDANATIVE_XMLSTOREFACTORY */
