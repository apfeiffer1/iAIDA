// Emacs -*- C++ -*-
#ifndef IAIDA_AIDA_HBOOKSTORE_HBOOKSTOREFACTORY
#define IAIDA_AIDA_HBOOKSTORE_HBOOKSTOREFACTORY 1

#include <string>
#include "AIDA_Dev/IStoreFactory.h"

namespace iAIDA {
  namespace AIDA_HBookStore {

/// Implementation of AIDA::DEV::IStoreFactory interface for a factory of hbook stores

class AIDA_HBookStoreFactory : virtual public AIDA::Dev::IStoreFactory
{
public:
  /// Constructor
  AIDA_HBookStoreFactory();
  /// Destructor
  ~AIDA_HBookStoreFactory();

  /// Method inherited from the IStoreFactory class
  AIDA::Dev::IStore* createStore( const std::string & storeName,
				  bool readOnly,
				  bool createNew,
				  const std::string & options );
  bool destroyStore( AIDA::Dev::IStore* store );
};

  }
}

#endif /* ifndef IAIDA_AIDA_HBOOKSTORE_HBOOKSTOREFACTORY */
