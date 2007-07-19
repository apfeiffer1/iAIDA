#include "AIDA_HBook/AIDA_HBookStoreFactory.h"
#include "AIDA_HBookStore.h"



#ifdef HBOOK_STORE_NO_EXCEPTIONS_TO_USER
#include <exception>
#include <iostream>
#define HBOOK_STORE_REPORT_ERROR( XXX ) std::cerr << XXX << std::endl
#endif


pi::AIDA_HBookStore::AIDA_HBookStoreFactory::AIDA_HBookStoreFactory()
{}

pi::AIDA_HBookStore::AIDA_HBookStoreFactory::~AIDA_HBookStoreFactory()
{}


AIDA::Dev::IStore*
pi::AIDA_HBookStore::AIDA_HBookStoreFactory::createStore( const std::string & storeName,
							      bool readOnly,
							      bool createNew,
							      const std::string & options )
{
#ifdef HBOOK_STORE_NO_EXCEPTIONS_TO_USER
  try {
#endif
    return new pi::AIDA_HBookStore::AIDA_StoreHBook( storeName, readOnly, createNew, options );
#ifdef HBOOK_STORE_NO_EXCEPTIONS_TO_USER
  }
  catch ( std::exception& e ) {
    HBOOK_STORE_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


bool
pi::AIDA_HBookStore::AIDA_HBookStoreFactory::destroyStore( AIDA::Dev::IStore* store )
{
  if ( store ) {
    delete store;
    return true;
  }
  else {
    return false;
  }
}

