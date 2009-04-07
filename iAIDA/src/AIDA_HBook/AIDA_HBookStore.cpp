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

#include "AIDA_HBookStore.h"
#include "HBook.h"
#include "HBookMemoryCache.h"
#include "HBookFileHandler.h"
#include "HBookDirectoryManager.h"
#include "HBookOptionParser.h"
#include "SupportedAIDATypes.h"
#include "HistogramConverter.h"
#include "RWNtuple.h"
#include "CWNtuple.h"

#include "AIDA_Dev/IDevManagedObject.h"

//-ap #include "AIDA_Plugin/AIDA_PluginLoader.h"
//-ap #include "AIDA_Plugin/AIDA_PluginType.h"

#include "AIDA_Histogram_native/AIDA_DevHistogramFactory.h"
#include "AIDA_Tuple_native/AIDA_DevTupleFactory.h"

#include "AIDA_Dev/IDevHistogramFactory.h"
#include "AIDA_Dev/IDevHistogram1D.h"
#include "AIDA_Dev/IDevHistogram2D.h"
#include "AIDA_Dev/IDevProfile1D.h"

#include "AIDA_Dev/IDevTupleFactory.h"
#include "AIDA_Dev/IDevTuple.h"
#include "AIDA_Dev/ITupleHeader.h"
#include "AIDA_Dev/ITupleVariableDescription.h"
#include "AIDA_Dev/ITupleVariableStatistics.h"

#include "AIDA/IAxis.h"

#include <memory>
#include <stdexcept>

#ifdef OLDSTREAMS
# include <strstream>
# define ostringstream ostrstream
# define istringstream istrstream
#else
# include <sstream>
#endif

#ifdef HBOOK_STORE_NO_EXCEPTIONS_TO_USER
#include <iostream>
#define HBOOK_STORE_REPORT_ERROR( XXX ) std::cerr << XXX << std::endl
#else
#define HBOOK_STORE_REPORT_ERROR( XXX ) throw std::runtime_error( XXX )
#endif

static const std::string float_type = "float";

static const std::string flagForRWNtuple = "--RWN";

static const std::string defaultTuplePluginType = "AIDA_Tuple_HBook"; 
static const std::string defaultHistoPluginType = "AIDA_Histogram_HBook"; 


iAIDA::AIDA_HBookStore::AIDA_StoreHBook::AIDA_StoreHBook( const std::string& name,
							   bool readOnly,
							   bool createNew,
							   const std::string& options ):
  m_name( name ),
  m_isClosed( true ),
  m_optionParser( new iAIDA::AIDA_HBookStore::HBookOptionParser( options ) ),
  m_fileHandler( 0 ),
  m_cache( 0 ),
  m_dirManager( 0 )
{

  // set plugins to be used 
  tuplePluginType = defaultTuplePluginType;   
  histoPluginType = defaultHistoPluginType;

  // Initialize HBOOK
  iAIDA::AIDA_HBookStore::HBook::initialize();

  // Open the file
  m_fileHandler = new iAIDA::AIDA_HBookStore::HBookFileHandler( m_name,
								 m_optionParser->recordLength(),
								 readOnly,
								 createNew );
  m_isClosed = false;

  // Create the memory cache
  m_cache = new iAIDA::AIDA_HBookStore::HBookMemoryCache( m_fileHandler->zebraName() );

  // Copy the directory structure from the file into the memory
  m_dirManager = new iAIDA::AIDA_HBookStore::HBookDirectoryManager( m_fileHandler->zebraName() );

  // Retrieve the headers of the objects
  collectObjectTypes( "/" );
}


iAIDA::AIDA_HBookStore::AIDA_StoreHBook::~AIDA_StoreHBook()
{
  close();
  if ( m_optionParser ) delete m_optionParser;
  for ( std::map< std::string, IPersistentTuple* >::iterator iTuple = m_tuples.begin();
	iTuple != m_tuples.end(); ++iTuple ) {
    if ( iTuple->second ) {
      delete iTuple->second;
    }
  }
}


const std::string&
iAIDA::AIDA_HBookStore::AIDA_StoreHBook::name() const
{
  return m_name;
}


bool
iAIDA::AIDA_HBookStore::AIDA_StoreHBook::writeObject( const AIDA::IManagedObject& dataObject,
						       const std::string& path )
{
  if ( m_objectTypes.find( path ) != m_objectTypes.end() ) return false;
  AIDA::Dev::IDevManagedObject* object =
    dynamic_cast< AIDA::Dev::IDevManagedObject* >( &( const_cast<AIDA::IManagedObject&>( dataObject ) ) );
  if ( ! object ) return false;
  const std::string& type = object->userLevelClassType();
  if ( ! iAIDA::AIDA_HBookStore::SupportedAIDATypes::supportedTypes().isTypeSupported( type ) ) {
    return false;
  }
  object->setUpToDate( false );
  if ( type == "IProfile1D" ) {
    AIDA::IProfile1D* p = dynamic_cast< AIDA::IProfile1D* >( object );
    if ( !p ) return false;
    if ( ! (  p->axis().isFixedBinning() ) ) return false;
  }
  else if ( type == "IHistogram2D" ) {
    AIDA::IHistogram2D* p = dynamic_cast< AIDA::IHistogram2D* >( object );
    if ( !p ) return false;
    if ( ! (  p->xAxis().isFixedBinning() ) ) return false;
    if ( ! (  p->yAxis().isFixedBinning() ) ) return false;
  }

  m_objectTypes[ path ] = type;
  m_objectRefs[ path ] = object;

  if ( m_objectsToDelete.find( path ) != m_objectsToDelete.end() ) {
    m_objectsToDelete.erase( path );
    m_objectsToUpdate.insert( path );
  }
  else {
    m_objectsToAdd.insert( path );
  }

  return true;
}


AIDA::IManagedObject*
iAIDA::AIDA_HBookStore::AIDA_StoreHBook::copyAndWrite( const AIDA::IManagedObject& dataObject,
							const std::string& path )
{
  if ( m_objectTypes.find( path ) != m_objectTypes.end() ) return 0;
  AIDA::Dev::IDevManagedObject* object =
    dynamic_cast< AIDA::Dev::IDevManagedObject* >( &( const_cast<AIDA::IManagedObject&>( dataObject ) ) );
  if ( ! object ) return 0;
  const std::string& type = object->userLevelClassType();
  if ( ! iAIDA::AIDA_HBookStore::SupportedAIDATypes::supportedTypes().isTypeSupported( type ) ) {
    return 0;
  }

//-ap   // get plugin loader
//-ap   AIDA_Plugin::AIDA_PluginLoader & pl = AIDA_Plugin::AIDA_PluginManager::instance();

  AIDA::Dev::IDevManagedObject* newObject = 0;


//-ap   // load the histogram plugin and create a  dev histo fractory
//-ap   std::auto_ptr<AIDA_Plugin::AIDA_PluginType>   plugin( pl.load(histoPluginType) ) ; 
//-ap   if ( ! plugin.get() ) {
//-ap     HBOOK_STORE_REPORT_ERROR( "The HBook store could not load a plugin of type " + histoPluginType );
//-ap     return 0; 
//-ap   }
//-ap   std::auto_ptr<AIDA::Dev::IDevHistogramFactory> hf(plugin->createDevHistogramFactory() ); 
//-ap   if ( ! hf.get() ) {
//-ap     HBOOK_STORE_REPORT_ERROR( "The HBook store could not create a dev histogram factory" );
//-ap     return 0; 
//-ap   }

  
  std::auto_ptr<AIDA::Dev::IDevHistogramFactory> hf( new iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory() ); 
  if ( ! hf.get() ) {
    HBOOK_STORE_REPORT_ERROR( "The HBook store could not create a dev histogram factory" );
    return 0; 
  }

  
  if ( type == "IHistogram1D" ) {
    newObject = hf->createCopy( dynamic_cast<const AIDA::IHistogram1D&>( dataObject ) );
  }
  else if ( type == "IHistogram2D" ) {
    newObject = hf->createCopy( dynamic_cast<const AIDA::IHistogram2D&>( dataObject ) );
  }
  else if ( type == "IProfile1D" ) {
    newObject = hf->createCopy( dynamic_cast<const AIDA::IProfile1D&>( dataObject ) );
  }

  // Identify the name
  unsigned int pos = path.size();
  for ( unsigned int iChar = path.size() - 1; iChar >= 0; --iChar ) {
    if ( path[iChar] == '/' ) break;
    --pos;
  }
  const std::string name = path.substr( pos );

  // Register the new object
  if ( newObject ) {
    newObject->setName( name );
    writeObject( *newObject, path );
  }
  return newObject;
}


AIDA::IManagedObject*
iAIDA::AIDA_HBookStore::AIDA_StoreHBook::retrieveObject( const std::string & path )
{
  std::map< std::string, AIDA::Dev::IDevManagedObject* >::iterator iObjectRef = m_objectRefs.find( path );
  if ( iObjectRef == m_objectRefs.end() ) return 0;
  if ( iObjectRef->second != 0 ) return iObjectRef->second;

  std::pair< std::string, int > directoryAndId = directoryAndIdentifier( iObjectRef->first );

//-ap  // get plugin loader
//-ap  AIDA_Plugin::AIDA_PluginLoader & pl = AIDA_Plugin::AIDA_PluginManager::instance();


  AIDA::Dev::IDevManagedObject* newObject = 0;
  const std::string& type = m_objectTypes[ iObjectRef->first ];
  if ( type == "ITuple" ) {

//-ap    // load the tuple plugin 
//-ap    std::auto_ptr<AIDA_Plugin::AIDA_PluginType>   plugin( pl.load(tuplePluginType) ) ; 
//-ap    if ( ! plugin.get() ) {
//-ap      HBOOK_STORE_REPORT_ERROR( "The HBook store could not load a plugin of type " + tuplePluginType );
//-ap      return 0; 
//-ap    }
//-ap    std::auto_ptr<AIDA::Dev::IDevTupleFactory> tf(plugin->createDevTupleFactory() ); 
//-ap    if ( ! tf.get() ) {
//-ap      HBOOK_STORE_REPORT_ERROR( "The HBook store could not create a dev tuple factory" );
//-ap      return 0; 
//-ap    }


    std::auto_ptr<AIDA::Dev::IDevTupleFactory> tf( new iAIDA::AIDA_Tuple_native::AIDA_DevTupleFactory() ); 

    AIDA::Dev::IDevTuple* tuple = tf->create( this, "" );
    if ( ! tuple ) {
      HBOOK_STORE_REPORT_ERROR( "Could not create a tuple" );
      return 0;
    }
    tuple->header().setPathInStore( path );
    m_objectRefs[path] = tuple;
    if ( ! this->readTupleHeader( tuple->header() ) ) {
      m_objectRefs[path] = 0;
      HBOOK_STORE_REPORT_ERROR( "Could not read the header of the tuple from the store" );
      return 0;
    }
    else newObject = tuple;
  }
  else {
//-ap     // load the histogram plugin and create a  dev histo fractory
//-ap     std::auto_ptr<AIDA_Plugin::AIDA_PluginType>   plugin( pl.load(histoPluginType) ) ; 
//-ap     if ( ! plugin.get() ) {
//-ap       HBOOK_STORE_REPORT_ERROR( "The HBook store could not load a plugin of type " + histoPluginType );
//-ap       return 0; 
//-ap     }
//-ap     std::auto_ptr<AIDA::Dev::IDevHistogramFactory> hf(plugin->createDevHistogramFactory() ); 
//-ap     if ( ! hf.get() ) {
//-ap       HBOOK_STORE_REPORT_ERROR( "The HBook store could not create a dev histogram factory" );
//-ap       return 0; 
//-ap     }

    std::auto_ptr<AIDA::Dev::IDevHistogramFactory> hf( new iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory() ); 
    if ( ! hf.get() ) {
      HBOOK_STORE_REPORT_ERROR( "The HBook store could not create a dev histogram factory" );
      return 0; 
    }


    iAIDA::AIDA_HBookStore::HBook::changeDirectory( m_cache->directoryInMemory( directoryAndId.first ) );
    iAIDA::AIDA_HBookStore::HBook::changeDirectory( m_cache->directoryInFile( directoryAndId.first ) );
    iAIDA::AIDA_HBookStore::HBook::loadObjectIntoMemory( directoryAndId.second );
    iAIDA::AIDA_HBookStore::HBook::changeDirectory( m_cache->directoryInMemory( directoryAndId.first ) );
    const iAIDA::AIDA_HBookStore::HistogramConverter& histogramConverter =
      iAIDA::AIDA_HBookStore::HistogramConverter::theConverter();
    if ( type == "IHistogram1D" ) {
      newObject = histogramConverter.createHistogram1DFromHBook( *hf, directoryAndId.second );
    }
    else if ( type == "IHistogram2D" ) {
      newObject = histogramConverter.createHistogram2DFromHBook( *hf, directoryAndId.second );
    }
    else if ( type == "IProfile1D" ) {
      newObject = histogramConverter.createProfile1DFromHBook( *hf, directoryAndId.second );
    }
    iAIDA::AIDA_HBookStore::HBook::deleteFromMemory( directoryAndId.second );
    m_objectRefs[path] = newObject;
  }

  // Register the new object
  if ( newObject ) {
    std::ostringstream os;
    os << directoryAndId.second;
#ifndef BADENDS
    os << std::ends;
#endif
    newObject->setUpToDate( true );
    newObject->setName( os.str() );
  }
  return newObject;
}


bool
iAIDA::AIDA_HBookStore::AIDA_StoreHBook::removeObject( const std::string& path )
{
  if ( m_objectTypes.find( path ) == m_objectTypes.end() ) return false;
  m_objectTypes.erase( path );
  m_objectRefs.erase( path );

  if ( m_objectsToAdd.find( path ) != m_objectsToAdd.end() ) {
    m_objectsToAdd.erase( path );
  }
  else {
    if ( m_objectsToUpdate.find( path ) != m_objectsToUpdate.end() ) {
      m_objectsToUpdate.erase( path );
    }
    m_objectsToDelete.insert( path );
  }
  return true;
}


bool
iAIDA::AIDA_HBookStore::AIDA_StoreHBook::moveObject(const std::string& from,
						     const std::string& to )
{
  if ( m_objectTypes.find( from ) == m_objectTypes.end() ) return false;
  if ( m_objectTypes.find( to ) != m_objectTypes.end() ) return false;
  if ( m_objectTypes[from] == "ITuple" ) return false;
  writeObject( *( retrieveObject( from ) ), to );
  return removeObject( from );
}


std::vector< std::string >
iAIDA::AIDA_HBookStore::AIDA_StoreHBook::listObjectPaths( const std::string directory,
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


std::vector< std::string > iAIDA::AIDA_HBookStore::AIDA_StoreHBook::listObjectTypes( const std::string directory,
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
iAIDA::AIDA_HBookStore::AIDA_StoreHBook::commit()
{
  if ( m_isClosed ) return false;
  std::set< std::string > tuplesToUpdate;
  // Loop over the rest of the objects and check whether they have been updated
  for ( std::map< std::string, AIDA::Dev::IDevManagedObject* >::iterator iObj = m_objectRefs.begin();
	iObj != m_objectRefs.end(); ++iObj ) {
    AIDA::Dev::IDevManagedObject* object = iObj->second;
    if ( object && !( object->isUpToDate() ) ) {
      if ( m_objectTypes[ iObj->first ] != "ITuple" ) {
	if ( m_objectsToAdd.find( iObj->first ) == m_objectsToAdd.end() ) {
	  m_objectsToDelete.insert( iObj->first );
	}
	m_objectsToAdd.insert( iObj->first );
      }
      else {
	tuplesToUpdate.insert( iObj->first );
      }
      object->setUpToDate( true );
    }
  }
  m_objectsToUpdate.clear();

  // Delete old objects.
  commitDelete();

  // Insert the new ones.
  commitAdd();

  // OK, now flush to disk keeping in memory only the tuples.
  m_cache->flushToFile( tuplesToUpdate );
  return true;
}


bool
iAIDA::AIDA_HBookStore::AIDA_StoreHBook::close()
{
  if ( m_isClosed ) return true;
  if ( m_cache ) {
    delete m_cache;
    m_cache = 0;
  }
  if ( m_dirManager ) {
    delete m_dirManager;
    m_dirManager = 0;
  }
  if ( m_fileHandler ) {
    delete m_fileHandler;
    m_fileHandler = 0;
  }
  m_isClosed = true;
  return true;
}


bool iAIDA::AIDA_HBookStore::AIDA_StoreHBook::canMoveTuples() const {return false;}
bool iAIDA::AIDA_HBookStore::AIDA_StoreHBook::canCopyTuples() const {return false;}

bool
iAIDA::AIDA_HBookStore::AIDA_StoreHBook::writeTupleHeader( AIDA::Dev::ITupleHeader& header )
{
  const std::string& pathInStore = header.pathInStore();
  if ( m_tuples.find( pathInStore ) != m_tuples.end() ) return false;
  if ( pathInStore[0] == '/' ) {
    std::pair<std::string, int> dirAndId = directoryAndIdentifier( pathInStore );
    bool noFloat = false;
    std::vector< std::string > variableNames;
    std::vector< std::string > variableTypes;
    for ( int iVar = 0; iVar < header.numberOfVariables(); ++iVar ) {
      const AIDA::Dev::ITupleVariableDescription* description = header.variableDescription( iVar );
      variableNames.push_back( description->variableName() );
      variableTypes.push_back( description->variableType() );
      if ( variableTypes.back() != float_type ) noFloat = true;
    }
    if ( variableNames.size() == 0 ) return false;

    // Create the target directory
    if ( ! m_dirManager->existsDirectory( dirAndId.first ) ) {
      if ( ! m_dirManager->createDirectory( dirAndId.first ) ) return false;
    }
    iAIDA::AIDA_HBookStore::HBook::changeDirectory( m_cache->directoryInMemory( dirAndId.first ) );
    iAIDA::AIDA_HBookStore::HBook::changeDirectory( m_cache->directoryInFile( dirAndId.first ) );

    if ( noFloat || header.options().find( flagForRWNtuple ) == std::string::npos ) { // CWN tuples
      iAIDA::AIDA_HBookStore::HBook::declareCWNTuple( dirAndId.second,
						       dynamic_cast< AIDA::ITuple* >( m_objectRefs[pathInStore] )->title() );
      iAIDA::AIDA_HBookStore::CWNtuple* cwntuple = new iAIDA::AIDA_HBookStore::CWNtuple( m_cache->directoryInMemory( dirAndId.first ),
											   dirAndId.second );

//-ap      // get plugin loader
//-ap      AIDA_Plugin::AIDA_PluginLoader & pl = AIDA_Plugin::AIDA_PluginManager::instance();
//-ap      // load the tuple plugin 
//-ap      std::auto_ptr<AIDA_Plugin::AIDA_PluginType>   plugin( pl.load(tuplePluginType) ) ; 
//-ap      if ( ! plugin.get() ) {
//-ap	HBOOK_STORE_REPORT_ERROR( "The HBook store could not load a plugin of type " + tuplePluginType );
//-ap	return 0; 
//-ap      }
//-ap      std::auto_ptr<AIDA::Dev::IDevTupleFactory> tf(plugin->createDevTupleFactory() ); 
//-ap      if ( ! tf.get() ) {
//-ap	HBOOK_STORE_REPORT_ERROR( "The HBook store could not create a dev tuple factory" );
//-ap	return 0; 
//-ap      }

      std::auto_ptr<AIDA::Dev::IDevTupleFactory> tf( new iAIDA::AIDA_Tuple_native::AIDA_DevTupleFactory() ); 

      cwntuple->writeDescription( header, *tf, this );
      m_tuples.insert( std::make_pair( pathInStore, static_cast<iAIDA::AIDA_HBookStore::IPersistentTuple*>( cwntuple ) ) );
      return true;
    }
    else { // RWN tuples
      if ( ! iAIDA::AIDA_HBookStore::HBook::bookRWNtuple( dirAndId.second,
							   dynamic_cast< AIDA::ITuple* >( m_objectRefs[pathInStore] )->title(),
							   variableNames,
							   m_cache->directoryInFile( dirAndId.first ),
							   m_optionParser->bufferSizeForRWN() ) ) return false;
      m_tuples.insert( std::make_pair( pathInStore,
				       static_cast<iAIDA::AIDA_HBookStore::IPersistentTuple*>( new iAIDA::AIDA_HBookStore::RWNtuple( m_cache->directoryInMemory( dirAndId.first ),
																       dirAndId.second,
																       variableNames.size() ) ) ) );
      return true;
    }
  }
  else {
    return false; // Something funny must have happened here !!!
  }
}


bool
iAIDA::AIDA_HBookStore::AIDA_StoreHBook::readTupleHeader( AIDA::Dev::ITupleHeader& header )
{
  const std::string& pathInStore = header.pathInStore();
  if ( m_tuples.find( pathInStore ) != m_tuples.end() ) return true;
  if ( pathInStore[0] == '/' ) {
    std::pair<std::string, int> dirAndId = directoryAndIdentifier( pathInStore );
    if ( ! m_dirManager->existsDirectory( dirAndId.first ) ) return false;
    std::vector< int > tuplesInDirectory = m_dirManager->tuples( dirAndId.first );
    bool tupleFound = false;
    for ( std::vector< int >::const_iterator iTuple = tuplesInDirectory.begin();
	  iTuple != tuplesInDirectory.end(); ++iTuple ) {
      if ( *iTuple == dirAndId.second ) {
	tupleFound = true;
	break;
      }
    }
    if ( ! tupleFound ) return false;

//-ap    // get plugin loader
//-ap    AIDA_Plugin::AIDA_PluginLoader & pl = AIDA_Plugin::AIDA_PluginManager::instance();
//-ap
//-ap    // load the tuple plugin 
//-ap    std::auto_ptr<AIDA_Plugin::AIDA_PluginType>   plugin( pl.load(tuplePluginType) ) ; 
//-ap    if ( ! plugin.get() ) {
//-ap      HBOOK_STORE_REPORT_ERROR( "The HBook store could not load a plugin of type " + tuplePluginType );
//-ap      return 0; 
//-ap    }
//-ap    std::auto_ptr<AIDA::Dev::IDevTupleFactory> tf(plugin->createDevTupleFactory() ); 
//-ap    if ( ! tf.get() ) {
//-ap      HBOOK_STORE_REPORT_ERROR( "The HBook store could not create a dev tuple factory" );
//-ap      return 0; 
//-ap    }

    std::auto_ptr<AIDA::Dev::IDevTupleFactory> tf( new iAIDA::AIDA_Tuple_native::AIDA_DevTupleFactory() ); 


    iAIDA::AIDA_HBookStore::HBook::changeDirectory( m_cache->directoryInMemory( dirAndId.first ) );
    iAIDA::AIDA_HBookStore::HBook::changeDirectory( m_cache->directoryInFile( dirAndId.first ) );
    iAIDA::AIDA_HBookStore::HBook::loadObjectIntoMemory( dirAndId.second );
    iAIDA::AIDA_HBookStore::HBook::changeDirectory( m_cache->directoryInMemory( dirAndId.first ) );

    // Retrieve the title and the number of rows
    const int numberOfEntries = iAIDA::AIDA_HBookStore::HBook::numberOfEntries( dirAndId.second );
    std::string title;
    std::vector<std::string> variableNames;
    std::vector<std::pair<float,float> > columnMinAndMax;
    iAIDA::AIDA_HBookStore::HBook::getTupleParameters( dirAndId.second, title, variableNames, columnMinAndMax );
    dynamic_cast< AIDA::Dev::IDevTuple* >( m_objectRefs[ pathInStore ] )->setTitle( title );
    header.setNumberOfRows( numberOfEntries );
    header.setCurrentRowNumber( -1 );

    // Build the descriptions
    if ( iAIDA::AIDA_HBookStore::HBook::isCWNtuple( dirAndId.second ) ) { // CWN tuple
      iAIDA::AIDA_HBookStore::CWNtuple* cwntuple = new iAIDA::AIDA_HBookStore::CWNtuple( m_cache->directoryInMemory( dirAndId.first ),
											   dirAndId.second );
      cwntuple->readDescription( header, *tf, this );
      m_tuples.insert( std::make_pair( pathInStore, static_cast<iAIDA::AIDA_HBookStore::IPersistentTuple*>( cwntuple ) ) );
      return true;
    }
    else { // RWN tuple
      for ( unsigned int i = 0; i < variableNames.size(); ++i ) {
	AIDA::Dev::ITupleVariableDescription * description = tf->createDescription();
	description->setVariableName( variableNames[i] );
	description->setVariableType( float_type );
	description->statistics().setStatistics( columnMinAndMax[i].first,
						 columnMinAndMax[i].second,
						 0, 0, numberOfEntries );
	header.setVariableDescription( description, true );
      }
      m_tuples.insert( std::make_pair( pathInStore, static_cast<iAIDA::AIDA_HBookStore::IPersistentTuple*>( new iAIDA::AIDA_HBookStore::RWNtuple( m_cache->directoryInMemory( dirAndId.first ),
																		    dirAndId.second,
																		    variableNames.size() ) ) ) );
      return true;
    }
  }  
  return false;
}


bool
iAIDA::AIDA_HBookStore::AIDA_StoreHBook::bindVariable( AIDA::Dev::ITupleHeader& header, int variableIndex )
{
  const std::string& pathInStore = header.pathInStore();
  if ( pathInStore[0] == '/' ) {
    std::map< std::string, iAIDA::AIDA_HBookStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( pathInStore );
    if ( iTuple == m_tuples.end() ) return false;
    else return iTuple->second->bindVariable( variableIndex );
  }
  else {
    const std::string path = pathInStore.substr( pathInStore.find( "/" ) );
    std::map< std::string, iAIDA::AIDA_HBookStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( path );
    if ( iTuple == m_tuples.end() ) return false;
    else return iTuple->second->bindVariable( variableIndex, pathInStore );
  }
}


void*
iAIDA::AIDA_HBookStore::AIDA_StoreHBook::variableAddress( AIDA::Dev::ITupleHeader& header, int variableIndex )
{
  const std::string& pathInStore = header.pathInStore();
  if ( pathInStore[0] == '/' ) {
    std::map< std::string, iAIDA::AIDA_HBookStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( pathInStore );
    if ( iTuple == m_tuples.end() ) return 0;
    else return iTuple->second->variableAddress( variableIndex );
  }
  else {
    const std::string path = pathInStore.substr( pathInStore.find( "/" ) );
    std::map< std::string, iAIDA::AIDA_HBookStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( path );
    if ( iTuple == m_tuples.end() ) return 0;
    else {
      void * p = iTuple->second->variableAddress( variableIndex, pathInStore );
      return p;
    }
  }
}


const void*
iAIDA::AIDA_HBookStore::AIDA_StoreHBook::variableAddress( const AIDA::Dev::ITupleHeader& header, int variableIndex ) const
{
  const std::string& pathInStore = header.pathInStore();
  if ( pathInStore[0] == '/' ) {
    std::map< std::string, iAIDA::AIDA_HBookStore::IPersistentTuple* >::const_iterator iTuple = m_tuples.find( pathInStore );
    if ( iTuple == m_tuples.end() ) return 0;
    else return iTuple->second->variableAddress( variableIndex );
  }
  else {
    const std::string path = pathInStore.substr( pathInStore.find( "/" ) );
    std::map< std::string, iAIDA::AIDA_HBookStore::IPersistentTuple* >::const_iterator iTuple = m_tuples.find( path );
    if ( iTuple == m_tuples.end() ) return 0;
    else  {
      const void * p = iTuple->second->variableAddress( variableIndex, pathInStore );
      return p;
    }
  }
}


bool
iAIDA::AIDA_HBookStore::AIDA_StoreHBook::clearBindings( const AIDA::Dev::ITupleHeader& header )
{
  const std::string& pathInStore = header.pathInStore();
  if ( pathInStore[0] == '/' ) {
    std::map< std::string, iAIDA::AIDA_HBookStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( pathInStore );
    if ( iTuple == m_tuples.end() ) return false;
    else return iTuple->second->clearBindings();
  }
  else {
    const std::string path = pathInStore.substr( pathInStore.find( "/" ) );
    std::map< std::string, iAIDA::AIDA_HBookStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( path );
    if ( iTuple == m_tuples.end() ) return false;
    else return iTuple->second->clearBindings( pathInStore );
  }
}


bool
iAIDA::AIDA_HBookStore::AIDA_StoreHBook::writeTupleRow( AIDA::Dev::ITupleHeader& header )
{
  const std::string& pathInStore = header.pathInStore();
  if ( pathInStore[0] == '/' ) {
    std::map< std::string, iAIDA::AIDA_HBookStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( pathInStore );
    if ( iTuple == m_tuples.end() ) return false;
    else return iTuple->second->writeTupleRow( header.currentRowNumber() );
  }
  else {
    const std::string path = pathInStore.substr( pathInStore.find( "/" ) );
    std::map< std::string, iAIDA::AIDA_HBookStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( path );
    if ( iTuple == m_tuples.end() ) return false;
    else return iTuple->second->writeTupleRow( header.currentRowNumber(), pathInStore );
  }
}


bool
iAIDA::AIDA_HBookStore::AIDA_StoreHBook::readTupleRow( AIDA::Dev::ITupleHeader& header )
{
  const std::string& pathInStore = header.pathInStore();
  if ( pathInStore[0] == '/' ) {
    std::map< std::string, iAIDA::AIDA_HBookStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( pathInStore );
    if ( iTuple == m_tuples.end() ) return false;
    else return iTuple->second->readTupleRow( header.currentRowNumber() );
  }
  else {
    const std::string path = pathInStore.substr( pathInStore.find( "/" ) );
    std::map< std::string, iAIDA::AIDA_HBookStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( path );
    if ( iTuple == m_tuples.end() ) return false;
    else return iTuple->second->readTupleRow( header.currentRowNumber(), pathInStore );
  }
}


bool
iAIDA::AIDA_HBookStore::AIDA_StoreHBook::resetTuple( AIDA::Dev::ITupleHeader& header )
{
  const std::string& pathInStore = header.pathInStore();
  if ( pathInStore[0] == '/' ) {
    std::map< std::string, iAIDA::AIDA_HBookStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( pathInStore );
    if ( iTuple == m_tuples.end() ) return false;
    else return iTuple->second->reset();
  }
  else {
    const std::string path = pathInStore.substr( pathInStore.find( "/" ) );
    std::map< std::string, iAIDA::AIDA_HBookStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( path );
    if ( iTuple == m_tuples.end() ) return false;
    else return iTuple->second->reset( pathInStore );
  }
}


void
iAIDA::AIDA_HBookStore::AIDA_StoreHBook::collectObjectTypes( const std::string& directory )
{
  AIDA::Dev::IDevManagedObject* dummyObject = 0;

  std::vector< int > h1 = m_dirManager->histograms1D( directory );
  for ( unsigned int ih1 = 0; ih1 < h1.size(); ++ih1 ) {
    std::ostringstream os;
    os << directory << h1[ih1];
#ifndef BADENDS
    os << std::ends;
#endif
    const std::string path = os.str();
    m_objectTypes.insert( std::make_pair( path, std::string( "IHistogram1D" ) ) );
    m_objectRefs.insert( std::make_pair( path, dummyObject ) );
  }

  std::vector< int > h2 = m_dirManager->histograms2D( directory );
  for ( unsigned int ih2 = 0; ih2 < h2.size(); ++ih2 ) {
    std::ostringstream os;
    os << directory << h2[ih2];
#ifndef BADENDS
    os << std::ends;
#endif
    const std::string path = os.str();
    m_objectTypes.insert( std::make_pair( path, std::string( "IHistogram2D" ) ) );
    m_objectRefs.insert( std::make_pair( path, dummyObject ) );
  }

  std::vector< int > p = m_dirManager->profiles( directory );
  for ( unsigned int ip = 0; ip < p.size(); ++ip ) {
    std::ostringstream os;
    os << directory << p[ip];
#ifndef BADENDS
    os << std::ends;
#endif
    const std::string path = os.str();
    m_objectTypes.insert( std::make_pair( path, std::string( "IProfile1D" ) ) );
    m_objectRefs.insert( std::make_pair( path, dummyObject ) );
  }

  std::vector< int > t = m_dirManager->tuples( directory );
  for ( unsigned int it = 0; it < t.size(); ++it ) {
    std::ostringstream os;
    os << directory << t[it];
#ifndef BADENDS
    os << std::ends;
#endif
    const std::string path = os.str();
    m_objectTypes.insert( std::make_pair( path, std::string( "ITuple" ) ) );
    m_objectRefs.insert( std::make_pair( path, dummyObject ) );
  }

  std::vector< std::string > subdirs = m_dirManager->subDirectories( directory );
  for ( unsigned int i = 0; i < subdirs.size(); ++i ) {
    collectObjectTypes( directory + subdirs[i] + "/" );
  }
}


void
iAIDA::AIDA_HBookStore::AIDA_StoreHBook::commitDelete()
{
  for ( std::set<std::string>::const_iterator iObj = m_objectsToDelete.begin();
	iObj != m_objectsToDelete.end(); ++iObj ) {
    std::pair< std::string, int > directoryAndId = directoryAndIdentifier( *iObj );
    iAIDA::AIDA_HBookStore::HBook::changeDirectory( m_cache->directoryInFile( directoryAndId.first ) );
    iAIDA::AIDA_HBookStore::HBook::deleteObjectFromDiskDirectory( directoryAndId.second );
    std::map< std::string, IPersistentTuple* >::iterator iTuple = m_tuples.find( *iObj );
    if ( iTuple != m_tuples.end() ) {
      delete iTuple->second;
      m_tuples.erase( iTuple );
    }
    if ( m_dirManager->isDirectoryEmpty( directoryAndId.first ) ) {
      m_dirManager->removeDirectory( directoryAndId.first );
    }
  }
  m_objectsToDelete.clear();
}


void
iAIDA::AIDA_HBookStore::AIDA_StoreHBook::commitAdd()
{
  for ( std::set<std::string>::const_iterator iObj = m_objectsToAdd.begin();
	iObj != m_objectsToAdd.end(); ++iObj ) {
    AIDA::Dev::IDevManagedObject* object = m_objectRefs[ *iObj ];
    const std::string& type = m_objectTypes[ *iObj ];
    if ( object && ( type != "ITuple"  ) ) {
      std::pair< std::string, int > directoryAndId = directoryAndIdentifier( *iObj );
      if ( ! m_dirManager->existsDirectory( directoryAndId.first ) ) {
	m_dirManager->createDirectory( directoryAndId.first );
      }
      iAIDA::AIDA_HBookStore::HBook::changeDirectory( m_cache->directoryInMemory( directoryAndId.first ) );
      const iAIDA::AIDA_HBookStore::HistogramConverter& histogramConverter =
	iAIDA::AIDA_HBookStore::HistogramConverter::theConverter();
      if ( type == "IHistogram1D" ) {
	AIDA::IHistogram1D* h = dynamic_cast<AIDA::IHistogram1D*>(object);
	if ( h ) histogramConverter.convertToHBook( *h, directoryAndId.second, m_optionParser->areErrorsStored() );
      }
      else if ( type == "IHistogram2D" ) {
	AIDA::IHistogram2D* h = dynamic_cast<AIDA::IHistogram2D*>(object);
	if ( h ) histogramConverter.convertToHBook( *h, directoryAndId.second, m_optionParser->areErrorsStored() );
      }
      else if ( type == "IProfile1D" ) {
	AIDA::IProfile1D* h = dynamic_cast<AIDA::IProfile1D*>(object);
	if ( h ) histogramConverter.convertToHBook( *h, directoryAndId.second );
      }
    }
    object->setUpToDate( true );
  }
  m_objectsToAdd.clear();
}


std::pair<std::string, int>
iAIDA::AIDA_HBookStore::AIDA_StoreHBook::directoryAndIdentifier( const std::string& path ) const
{
  unsigned int pos = path.size();
  for ( unsigned int iChar = path.size() - 1; iChar >= 0; --iChar ) {
    if ( path[iChar] == '/' ) break;
    --pos;
  }
  std::string name = path.substr( pos );
  std::string directory = path.substr( 0, pos - 1 );
  if ( directory == "" ) directory = "/";
  int id;
  std::istringstream is( name.c_str() );
  is >> id;
  return std::make_pair( directory, id );
}
