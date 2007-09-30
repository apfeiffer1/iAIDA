//
//    Copyright 2007 Andreas Pfeiffer
//
/*
 *  This file is part of iAIDA.
 *
 *  iAIDA is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as 
 *  published by the Free Software Foundation, either version 3 of 
 *  the License.
 *
 *  iAIDA is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public 
 *  License along with iAIDA.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "AIDA_Tree/AIDA_TreeFactory.h"
#include "AIDA_Tree.h"
#include "AIDA_Dev/IDevAnalysisFactory.h"
#include "AIDA_Dev/IStoreFactory.h"

#include "AIDA_MemoryStoreFactory.h"
#include "AIDA_XMLStoreFactory.h"

//-ap #include "AIDA_Plugin/AIDA_PluginLoader.h"
//-ap #include "AIDA_Plugin/AIDA_PluginType.h"
//-ap #include "SealBase/SharedLibrary.h"

#ifdef HAVE_ROOT
# include "AIDA_ROOT/AIDA_RootStoreFactory.h"
#endif

#ifdef HAVE_CERN
# include "AIDA_HBook/AIDA_HBookStoreFactory.h"
#endif


#include <memory>
#include <stdexcept>
#include <cctype>
#include <algorithm>

#ifdef IAIDA_TREE_NO_EXCEPTIONS_TO_USER
#include <exception>
#include <iostream>
#define TREE_REPORT_ERROR( XXX ) std::cerr << XXX << std::endl
#endif

iAIDA::aida_tree::AIDA_TreeFactory::AIDA_TreeFactory()
{}

iAIDA::aida_tree::AIDA_TreeFactory::~AIDA_TreeFactory()
{}

AIDA::ITree*
iAIDA::aida_tree::AIDA_TreeFactory::create()
{
  std::string storeName = "";
  return this->create( storeName );
}


AIDA::ITree*
iAIDA::aida_tree::AIDA_TreeFactory::create( const std::string & storeName,
						    const std::string & storeType,
						    bool readOnly,
						    bool createNew,
						    const std::string & options )
{
#ifdef IAIDA_TREE_NO_EXCEPTIONS_TO_USER
  try {
#endif

    std::string m_storeType = storeType;
    if ( m_storeType == "" ) m_storeType = "Memory";

    // transform to upper case
    std::transform( m_storeType.begin(), m_storeType.end(), m_storeType.begin(), toupper); 
    
    std::string storePluginType = "AIDA_Store_" + m_storeType; 
    
    /*-ap-begin
    // special treatment in the case of HBOOK: 
    // check if libshift.so is available
    if (m_storeType == "HBOOK") {
      // try to load libshift.so
      std::string libname = seal::SharedLibrary::libname ("shift");
      try { 
 	seal::SharedLibrary::load (libname);
	//std::cout << "libshift.so found and loaded " << std::endl; 
      }
      catch (seal::SharedLibraryError &error) { 
	//std::cerr << "AIDA_TreeFactory: Error loading plugin AIDA_Store_HBOOK - libshift is missing" << std::endl; 
	throw std::runtime_error( "The tree factory could not load a plugin of type " + storePluginType + " because libshift is missing");      
      }
    }
    ap-end-*/

    /* 
    // Create the local store : get first a store factory from plugin
    AIDA_Plugin::AIDA_PluginLoader & pl = AIDA_Plugin::AIDA_PluginManager::instance();
    // load the plugin 
    std::auto_ptr<AIDA_Plugin::AIDA_PluginType>   plugin( pl.load(storePluginType) ) ; 
    if ( ! plugin.get() ) { 
      throw std::runtime_error( "The tree factory could not load a plugin of type " + storePluginType );
    }
    std::auto_ptr<AIDA::Dev::IStoreFactory> sf(plugin->createStoreFactory() );  
    if ( ! sf.get() ) {
      throw std::runtime_error( "The tree factory could not create a factory for stores of type " + m_storeType );
    }
    */

    AIDA::Dev::IStoreFactory *sf_p=0;
    if (m_storeType == "MEMORY") {
      sf_p = new AIDA_MemoryStore::AIDA_MemoryStoreFactory();
#ifdef HAVE_ROOT
    } else if (m_storeType == "ROOT") {
      sf_p = new iAIDA::AIDA_RootStore::AIDA_RootStoreFactory();
#endif
#ifdef HAVE_CERN
    } else if (m_storeType == "HBOOK") {
      sf_p = new iAIDA::AIDA_HBookStore::AIDA_HBookStoreFactory();
#endif
    } else {
      std::cerr << "WARNING: Unknow store type " << m_storeType << " falling back to XMLStore !!! " << std::endl;
      sf_p = new AIDA_XMLStore::AIDA_XMLStoreFactory();
    }

    std::auto_ptr<AIDA::Dev::IStoreFactory> sf( sf_p );  
    AIDA::Dev::IStore* m_store = 0;
    m_store = sf->createStore( storeName, readOnly, createNew, options );
    if ( ! m_store ) {
      throw std::runtime_error( "The tree factory could not create a store of type " + m_storeType );
    }

    return new iAIDA::aida_tree::AIDA_Tree( m_store );
#ifdef IAIDA_TREE_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    TREE_REPORT_ERROR(">>> Error creating Tree " + storeName + " of type " + storeType + " <<<<\n" 
		      +  e.what() );
    return 0;
  }
#endif
}
