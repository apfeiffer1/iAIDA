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

#include "AIDA_MemoryStore.h"
#include "AIDA_Dev/IDevManagedObject.h"
#include "AIDA_Dev/IDevTupleFactory.h"

#include "AIDA_Histogram_native/AIDA_DevHistogramFactory.h"
#include "AIDA_DataPointSet_native/AIDA_DevDataPointSetFactory.h"
#include "AIDA_Tuple_native/AIDA_DevTupleFactory.h"
#include "AIDA_Function/AIDA_DevFunctionFactory.h"

#include "AIDA_Plugin/AIDA_PluginLoader.h"
#include "AIDA_Plugin/AIDA_PluginType.h"

#include <typeinfo>
#include <set>
#include <memory>
#include "MemoryHistogramCopier.h"
#include "MemoryFunctionCopier.h"
#include "MemoryDataPointSetCopier.h"
#include "AIDA_MemoryStore/MemoryBackingStore.h"
#include <iostream>

#ifdef MEMORY_STORE_NO_EXCEPTIONS_TO_USER
#include <iostream>
#define MEMORY_STORE_REPORT_ERROR( XXX ) std::cerr << XXX << std::endl
#else
#include <stdexcept>
#define MEMORY_STORE_REPORT_ERROR( XXX ) throw std::runtime_error( XXX )
#endif


static std::string defaultTuplePluginType = "AIDA_Tuple_Native"; 
static std::string defaultHistoPluginType = "AIDA_Histogram_Native"; 
static std::string defaultDPSPluginType = "AIDA_DataPointSet_Native"; 
static std::string defaultFuncPluginType = "AIDA_Function_Native"; 

iAIDA::AIDA_MemoryStore::AIDA_StoreMemory::AIDA_StoreMemory( const std::string& name ):
  m_name( name ),
  m_objectTypes(),
  m_backingStore( 0 )
{
  // set lugins to be used 
  tuplePluginType = defaultTuplePluginType;   
  histoPluginType = defaultHistoPluginType;
  dpsPluginType = defaultDPSPluginType;
  funcPluginType = defaultFuncPluginType;

  // create backing store (neeed to know tuple plugin type)  
  try { 
    m_backingStore = new iAIDA::AIDA_MemoryStore::MemoryBackingStore(tuplePluginType);
  } 
  catch (std::exception & e) {
    // do not want throw exception here - just print message
    // do not print msg
    //std::cout << "AIDA_Memory_Store: cannot create a backing store " << std::endl; 
  }
}


iAIDA::AIDA_MemoryStore::AIDA_StoreMemory::~AIDA_StoreMemory()
{
  if ( m_backingStore ) delete m_backingStore;
}


const std::string&
iAIDA::AIDA_MemoryStore::AIDA_StoreMemory::name() const
{
  return m_name;
}


bool
iAIDA::AIDA_MemoryStore::AIDA_StoreMemory::writeObject( const AIDA::IManagedObject& dataObject,
							 const std::string& path )
{
  try {
    const AIDA::Dev::IDevManagedObject& object = dynamic_cast< const AIDA::Dev::IDevManagedObject& >( dataObject );
    m_objectTypes[ path ] = object.userLevelClassType();

    // No corresponding persistent object to be created...
  }
  catch ( std::bad_cast ) {
    return false;
  }
  return false;
}


/// Here we need the AIDA types
AIDA::IManagedObject*
iAIDA::AIDA_MemoryStore::AIDA_StoreMemory::copyAndWrite( const AIDA::IManagedObject& dataObject,
							  const std::string& path )
{
  // Create the local store

  AIDA::Dev::IDevManagedObject* newObject = 0;
  // Identify the name
  unsigned int pos = path.size();
  for ( int iChar = path.size() - 1; iChar >= 0; --iChar ) {
    if ( path[iChar] == '/' ) break;
    --pos;
  }
  const std::string name = path.substr( pos );

  // Identify the type of the new object
  const AIDA::Dev::IDevManagedObject& object = dynamic_cast< const AIDA::Dev::IDevManagedObject& >( dataObject );
  const std::string& type = object.userLevelClassType();


  // Create the actual copy

  // Histogram/Profile/Cloud cases
  if ( type == "IHistogram1D" || type == "IHistogram2D" || type == "IHistogram3D" ||
       type == "IProfile1D" || type == "IProfile2D" ||
       type == "ICloud1D" || type == "ICloud2D" || type == "ICloud3D" ) {

    /*
    // load the histogram plugin 
    std::auto_ptr<AIDA_Plugin::AIDA_PluginType>   plugin( pl.load(histoPluginType) ) ; 
    if ( ! plugin.get() ) {
      MEMORY_STORE_REPORT_ERROR( "The memory store could not load a plugin of type " + histoPluginType );
      return 0; 
    }
    std::auto_ptr<AIDA::Dev::IDevHistogramFactory> hf(plugin->createDevHistogramFactory() ); 
    if ( ! hf.get() ) {
      MEMORY_STORE_REPORT_ERROR( "The memory store could not create a dev histogram factory" );
      return 0; 
    }
   */
    std::auto_ptr<AIDA::Dev::IDevHistogramFactory> hf( new AIDA_Histogram_native::AIDA_DevHistogramFactory() ); 
    
    iAIDA::AIDA_MemoryStore::MemoryHistogramCopier hp;
    newObject = hp.createCopy( dataObject, *hf, type );
  }
  else if ( type == "IDataPointSet" ) {  // DataPointSet cases

    /*
    // load the plugin  for DPS
    std::auto_ptr<AIDA_Plugin::AIDA_PluginType>   plugin( pl.load(dpsPluginType) ) ; 
    if ( ! plugin.get() ) {
      MEMORY_STORE_REPORT_ERROR( "The memory store could not load a plugin of type " + dpsPluginType );
      return 0; 
    }
    std::auto_ptr<AIDA::Dev::IDevDataPointSetFactory> df(plugin->createDevDataPointSetFactory() ); 
    if ( ! df.get() ) {
      MEMORY_STORE_REPORT_ERROR( "The memory store could not create a dev data point set  factory" );
      return 0; 
    }
    */
    std::auto_ptr<AIDA::Dev::IDevDataPointSetFactory> df(new AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory() ); 

    iAIDA::AIDA_MemoryStore::MemoryDataPointSetCopier hp;
    newObject = hp.createCopy( dataObject, *df );
  }
  else if ( type == "IFunction" ) { // Function cases

    /*
    // load the function plugin 
    std::auto_ptr<AIDA_Plugin::AIDA_PluginType>   plugin( pl.load(funcPluginType) ) ; 
    if ( ! plugin.get() ) {
      MEMORY_STORE_REPORT_ERROR( "The memory store could not load a plugin of type " + funcPluginType );
      return 0; 
    }
    std::auto_ptr<AIDA::Dev::IDevFunctionFactory> ff(plugin->createDevFunctionFactory() ); 
    if ( ! ff.get() ) {
      MEMORY_STORE_REPORT_ERROR( "The memory store could not create a dev function factory" );
      return 0; 
    }
    */
    std::auto_ptr<AIDA::Dev::IDevFunctionFactory> ff( new AIDA_Function::AIDA_DevFunctionFactory() ); 

    iAIDA::AIDA_MemoryStore::MemoryFunctionCopier hp;
    newObject = hp.createCopy( dataObject, *ff );
  }

  // Others to be added...

  // Register the new object
  if ( newObject ) {
    newObject->setName( name );
    writeObject( *newObject, path );
  }
  return newObject;
}


AIDA::IManagedObject*
iAIDA::AIDA_MemoryStore::AIDA_StoreMemory::retrieveObject( const std::string & )
{
  // Nothing to be retrieved from a memory store
  return 0;
}


bool
iAIDA::AIDA_MemoryStore::AIDA_StoreMemory::removeObject( const std::string& path )
{
  m_objectTypes.erase( path );
  // Nothing to be removed from a memory store
  return true;
}


bool
iAIDA::AIDA_MemoryStore::AIDA_StoreMemory::moveObject(const std::string& from, const std::string& to )
{
  std::map< std::string, std::string >::const_iterator sourceObject = m_objectTypes.find( from );
  if ( sourceObject == m_objectTypes.end() ) return false;
  std::string type = sourceObject->second;
  if ( type == "ITuple" ) return false;
  m_objectTypes.erase( from );
  m_objectTypes.insert( std::make_pair( to, type ) );
  return true;
}


std::vector< std::string >
iAIDA::AIDA_MemoryStore::AIDA_StoreMemory::listObjectPaths( const std::string directory,
							     bool recursive ) const
{
  std::string dir = directory;
  if ( dir[ dir.size() - 1 ] != '/' ) dir += "/";

  std::vector< std::string > result;

  std::set< std::string > directorySet;
  for ( std::map< std::string, std::string >::const_iterator iObj = m_objectTypes.begin();
	iObj != m_objectTypes.end(); ++iObj ) {
    const std::string& object = iObj->first;
    std::string::size_type p = object.find( dir );
    if ( p == 0 ) {
      std::string complement = object.substr( dir.size() );
      // Remove extra "/" characters in the begining
      while ( complement.size() > 0 && complement[0] == '/' ) complement = complement.substr( 1 );
      if ( complement.size() == 0 ) continue;

      // Check if it is a leaf object
      std::string::size_type n = complement.find( "/" );
      if ( n == std::string::npos ) {
	result.push_back( object );
      }
      else { // update the daughter directory list
	directorySet.insert( dir + complement.substr( 0, n ) + "/" );
      }
    }
  }

  for ( std::set< std::string >::const_iterator iDir = directorySet.begin();
	iDir != directorySet.end(); ++iDir ) {
    result.push_back( *iDir );
    if ( recursive ) {
      std::vector< std::string > subDirResults = listObjectPaths( *iDir, recursive );
      for ( unsigned int i = 0; i < subDirResults.size(); ++i ) result.push_back( subDirResults[ i ] );
    }
  }

  return result;
}


std::vector< std::string > iAIDA::AIDA_MemoryStore::AIDA_StoreMemory::listObjectTypes( const std::string directory,
											bool recursive ) const
{
  std::string dir = directory;
  if ( dir[ dir.size() - 1 ] != '/' ) dir += "/";

  std::vector< std::string > result;

  std::set< std::string > directorySet;
  for ( std::map< std::string, std::string >::const_iterator iObj = m_objectTypes.begin();
	iObj != m_objectTypes.end(); ++iObj ) {
    const std::string& object = iObj->first;
    std::string::size_type p = object.find( dir );
    if ( p == 0 ) {
      std::string complement = object.substr( dir.size() );
      // Remove extra "/" characters in the begining
      while ( complement.size() > 0 && complement[0] == '/' ) complement = complement.substr( 1 );
      if ( complement.size() == 0 ) continue;

      // Check if it is a leaf object
      std::string::size_type n = complement.find( "/" );
      if ( n == std::string::npos ) {
	result.push_back( iObj->second );
      }
      else { // update the daughter directory list
	directorySet.insert( dir + complement.substr( 0, n ) + "/" );
      }
    }
  }

  for ( std::set< std::string >::const_iterator iDir = directorySet.begin();
	iDir != directorySet.end(); ++iDir ) {
    result.push_back( "dir" );
    if ( recursive ) {
      std::vector< std::string > subDirResults = listObjectTypes( *iDir, recursive );
      for ( unsigned int i = 0; i < subDirResults.size(); ++i ) result.push_back( subDirResults[ i ] );
    }
  }

  return result;
}


bool
iAIDA::AIDA_MemoryStore::AIDA_StoreMemory::commit()
{
  // Nothing to commit in a memory store...
  return true;
}


bool
iAIDA::AIDA_MemoryStore::AIDA_StoreMemory::close()
{
  return true;
}


bool iAIDA::AIDA_MemoryStore::AIDA_StoreMemory::canMoveTuples() const {return false;}
bool iAIDA::AIDA_MemoryStore::AIDA_StoreMemory::canCopyTuples() const {return false;}


bool
iAIDA::AIDA_MemoryStore::AIDA_StoreMemory::writeTupleHeader( AIDA::Dev::ITupleHeader& header )
{
  if ( ! m_backingStore ) return false;
  else return m_backingStore->writeTupleHeader( header );
}


bool
iAIDA::AIDA_MemoryStore::AIDA_StoreMemory::readTupleHeader( AIDA::Dev::ITupleHeader& header )
{
  if ( ! m_backingStore ) return false;
  else return m_backingStore->readTupleHeader( header );
}


bool
iAIDA::AIDA_MemoryStore::AIDA_StoreMemory::bindVariable( AIDA::Dev::ITupleHeader& header, int variableIndex )
{
  if ( ! m_backingStore ) return false;
  else return m_backingStore->bindVariable( header, variableIndex );
}


void*
iAIDA::AIDA_MemoryStore::AIDA_StoreMemory::variableAddress( AIDA::Dev::ITupleHeader& header, int variableIndex )
{
  if ( ! m_backingStore ) return 0;
  else return m_backingStore->variableAddress( header, variableIndex );
}


const void*
iAIDA::AIDA_MemoryStore::AIDA_StoreMemory::variableAddress( const AIDA::Dev::ITupleHeader& header, int variableIndex ) const
{
  if ( ! m_backingStore ) return 0;
  else return m_backingStore->variableAddress( header, variableIndex );
}


bool
iAIDA::AIDA_MemoryStore::AIDA_StoreMemory::clearBindings( const AIDA::Dev::ITupleHeader& header )
{
  if ( ! m_backingStore ) return false;
  else return m_backingStore->clearBindings( header );
}


bool
iAIDA::AIDA_MemoryStore::AIDA_StoreMemory::writeTupleRow( AIDA::Dev::ITupleHeader& header )
{
  if ( ! m_backingStore ) return false;
  else return m_backingStore->writeTupleRow( header );
}


bool
iAIDA::AIDA_MemoryStore::AIDA_StoreMemory::readTupleRow( AIDA::Dev::ITupleHeader& header )
{
  if ( ! m_backingStore ) return false;
  else return m_backingStore->readTupleRow( header );
}


bool
iAIDA::AIDA_MemoryStore::AIDA_StoreMemory::resetTuple( AIDA::Dev::ITupleHeader& header )
{
  if ( ! m_backingStore ) return false;
  else return m_backingStore->resetTuple( header );
}

