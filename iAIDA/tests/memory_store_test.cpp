#include <iostream>
#include <stdexcept>
#include "AIDA_Dev/IDevAnalysisFactory.h"
#include "AIDA_Dev/IStoreFactory.h"
#include "AIDA_Dev/IStore.h"
#include "AIDA_Dev/IBackingStore.h"

#include "AIDA_Plugin/AIDA_PluginLoader.h"
#include "AIDA_Plugin/AIDA_PluginType.h"


#include <fstream>

int main(int, char**) {
  //std::ofstream fout( "memory_store_test.out" );
  std::ostream & fout = std::cout; 
  try {
  // need to load a tuple implementation (use plugin manager) 
    AIDA_Plugin::AIDA_PluginLoader & pl = AIDA_Plugin::AIDA_PluginManager::instance();
    // load the plugin 
    std::auto_ptr<AIDA_Plugin::AIDA_PluginType>   plugin( pl.load("AIDA_Store_MEMORY") ) ; 
    fout << "Fetching the store factory" << std::endl;
    AIDA::Dev::IStoreFactory* storeFactory = plugin->createStoreFactory( );
    if ( ! storeFactory ) throw std::runtime_error( "Could not fetch a store factory" );
    AIDA::Dev::IStore* store = storeFactory->createStore( "A memory store", true, true, "" );
    
    fout << "Creating the store" << std::endl;
    if ( ! store ) throw std::runtime_error( "Could not create a store" );
    fout << "Store name : " << store->name() << std::endl;
    AIDA::Dev::IBackingStore* backingstore = dynamic_cast<AIDA::Dev::IBackingStore*>( store );
    if ( !backingstore ) throw std::runtime_error( "Store is not a backing store" );
    fout << "Test succeeded !!!" << std::endl;
    return 0;
  }
  catch( std::exception & e ) {
    fout << e.what() << std::endl;
    return 1;
  }
}
