#include "AIDA_MemoryStoreFactory.h"
#include "AIDA_MemoryStore.h"


iAIDA::AIDA_MemoryStore::AIDA_MemoryStoreFactory::AIDA_MemoryStoreFactory()
{}

iAIDA::AIDA_MemoryStore::AIDA_MemoryStoreFactory::~AIDA_MemoryStoreFactory()
{}


AIDA::Dev::IStore*
iAIDA::AIDA_MemoryStore::AIDA_MemoryStoreFactory::createStore( const std::string & storeName,
								bool,
								bool,
								const std::string & )
{
  return new iAIDA::AIDA_MemoryStore::AIDA_StoreMemory( storeName );
}


bool
iAIDA::AIDA_MemoryStore::AIDA_MemoryStoreFactory::destroyStore( AIDA::Dev::IStore* store )
{
  if ( store ) {
    delete store;
    return true;
  }
  else {
    return false;
  }
}

