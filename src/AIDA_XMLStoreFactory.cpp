#include "AIDA_XMLStoreFactory.h"
#include "AIDA_StoreXML.h"



#ifdef XML_STORE_NO_EXCEPTIONS_TO_USER
#include <exception>
#include <iostream>
#define XML_STORE_REPORT_ERROR( XXX ) std::cerr << XXX << std::endl
#endif


iAIDA::AIDA_XMLStore::AIDA_XMLStoreFactory::AIDA_XMLStoreFactory()
{}

iAIDA::AIDA_XMLStore::AIDA_XMLStoreFactory::~AIDA_XMLStoreFactory()
{}


AIDA::Dev::IStore*
iAIDA::AIDA_XMLStore::AIDA_XMLStoreFactory::createStore( const std::string & storeName,
							  bool readOnly,
							  bool createNew,
							  const std::string & options )
{
#ifdef XML_STORE_NO_EXCEPTIONS_TO_USER
  try {
#endif
    return new iAIDA::AIDA_XMLStore::AIDA_StoreXML( storeName, readOnly, createNew, options );
#ifdef XML_STORE_NO_EXCEPTIONS_TO_USER
  }
  catch ( std::exception& e ) {
    XML_STORE_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


bool
iAIDA::AIDA_XMLStore::AIDA_XMLStoreFactory::destroyStore( AIDA::Dev::IStore* store )
{
  if ( store ) {
    delete store;
    return true;
  }
  else {
    return false;
  }
}

