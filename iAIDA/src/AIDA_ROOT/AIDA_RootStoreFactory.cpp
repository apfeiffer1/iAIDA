#include "AIDA_RootStoreFactory.h"
#include "AIDA_StoreRoot.h"


#ifdef Root_STORE_NO_EXCEPTIONS_TO_USER
#include <exception>
#include <iostream>
#define Root_STORE_REPORT_ERROR( XXX ) std::cerr << XXX << std::endl
#endif


iAIDA::AIDA_RootStore::AIDA_RootStoreFactory::AIDA_RootStoreFactory()
{}

iAIDA::AIDA_RootStore::AIDA_RootStoreFactory::~AIDA_RootStoreFactory()
{}


AIDA::Dev::IStore*
iAIDA::AIDA_RootStore::AIDA_RootStoreFactory::createStore( const std::string & storeName,
							  bool readOnly,
							  bool createNew,
							  const std::string & options )
{
#ifdef Root_STORE_NO_EXCEPTIONS_TO_USER
  try {
#endif
    return new iAIDA::AIDA_RootStore::AIDA_StoreRoot( storeName, readOnly, createNew, options );
#ifdef Root_STORE_NO_EXCEPTIONS_TO_USER
  }
  catch ( std::exception& e ) {
    Root_STORE_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


bool
iAIDA::AIDA_RootStore::AIDA_RootStoreFactory::destroyStore( AIDA::Dev::IStore* store )
{
  if ( store ) {
    delete store;
    return true;
  }
  else {
    return false;
  }
}

