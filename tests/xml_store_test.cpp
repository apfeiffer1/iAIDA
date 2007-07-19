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
  std::ostream& fout = std::cout;
  std::string storePluginType = "AIDA_Store_XML";
  try {
    fout << "Creating a plugin to fetch store factory" << std::endl;
  // Create the local store : get first a store factory from plugin
    AIDA_Plugin::AIDA_PluginLoader & pl = AIDA_Plugin::AIDA_PluginManager::instance();
    // load the plugin 
    std::auto_ptr<AIDA_Plugin::AIDA_PluginType>   plugin( pl.load(storePluginType) ) ; 
    if ( ! plugin.get() ) { 
      throw std::runtime_error( "The tree factory could not load a plugin of type " + storePluginType );
    }
    
    fout << "Fetching the store factory" << std::endl;
    std::auto_ptr<AIDA::Dev::IStoreFactory> storeFactory(plugin->createStoreFactory() );  
    if ( ! storeFactory.get() ) throw std::runtime_error( "Could not fetch a store factory" );

    fout << "Creating the store" << std::endl;
    AIDA::Dev::IStore* store = storeFactory->createStore( "mystore.xml", false, true, "" );
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
