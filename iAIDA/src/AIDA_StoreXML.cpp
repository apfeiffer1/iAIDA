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

#include "AIDA_StoreXML.h"
#include "StoreTranslator.h"
#include "Histo1DTranslator.h"
#include "Histo2DTranslator.h"
#include "Histo3DTranslator.h"
#include "Profile1DTranslator.h"
#include "Profile2DTranslator.h"
#include "Cloud1DTranslator.h"
#include "Cloud2DTranslator.h"
#include "Cloud3DTranslator.h"
#include "DataPointSetTranslator.h"
#include "FunctionTranslator.h"
#include "TupleTranslator.h"
#include "AIDA_Dev/IDevManagedObject.h"
#include "AIDA_Dev/ITupleFactoryProvider.h"
#include <typeinfo>
#include <set>
#include "AIDA_Dev/IDevHistogramFactory.h"
#include "AIDA_Dev/IDevHistogram1D.h"
#include "AIDA_Dev/IDevHistogram2D.h"
#include "AIDA_Dev/IDevHistogram3D.h"
#include "AIDA_Dev/IDevCloud1D.h"
#include "AIDA_Dev/IDevCloud2D.h"
#include "AIDA_Dev/IDevCloud3D.h"
#include "AIDA_Dev/IDevProfile1D.h"
#include "AIDA_Dev/IDevProfile2D.h"
#include "AIDA_Dev/IDevDataPointSet.h"
#include "AIDA_Dev/IDevFunction.h"
#include "AIDA_Dev/IDevTuple.h"
#include "AIDA_Dev/IHistogramFactoryProvider.h"
#include "AIDA_Dev/IDevDataPointSetFactory.h"
#include "AIDA_Dev/IDataPointSetFactoryProvider.h"
#include "AIDA_Dev/IDevFunctionFactory.h"
#include "AIDA_Dev/IFunctionFactoryProvider.h"
#include "AIDA_Dev/ITupleFactoryProvider.h"
#include "AIDA_Dev/IDevTupleFactory.h"
#include "AIDA_MemoryStore/MemoryBackingStore.h"

#include "AIDA_Histogram_native/AIDA_DevHistogramFactory.h"
#include "AIDA_DataPointSet_native/AIDA_DevDataPointSetFactory.h"
#include "AIDA_Function/AIDA_DevFunctionFactory.h"
#include "AIDA_Tuple_native/AIDA_DevTupleFactory.h"

//-ap #include "AIDA_Plugin/AIDA_PluginLoader.h"
//-ap #include "AIDA_Plugin/AIDA_PluginType.h"

#include <boost/algorithm/string.hpp>

#include "DataXML/DataObject.h"
// for compressing
#include "gzstream.h"
// for gzip and bzip2 compressing via boost
#include <fstream>
#include <iostream>

#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/filtering_stream.hpp>
// #include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/bzip2.hpp>

#include <memory>
#include <iostream>
#include <algorithm>

#ifdef XML_STORE_NO_EXCEPTIONS_TO_USER
#include <iostream>
#define XML_STORE_REPORT_ERROR( XXX ) std::cerr << XXX << std::endl
#else
#include <stdexcept>
#define XML_STORE_REPORT_ERROR( XXX ) throw std::runtime_error( XXX )
#endif


static std::string defaultTuplePluginType = "AIDA_Tuple_Native"; 
static std::string defaultHistoPluginType = "AIDA_Histogram_Native"; 
static std::string defaultDPSPluginType = "AIDA_DataPointSet_Native"; 
static std::string defaultFuncPluginType = "AIDA_Function_Native"; 


/* create the XML store
 *   - read always all the object - if file does not exists nothing 
 *       will be found 
 *   - always write again in a new file at commit all the objects    
 *     so createNew has no influence
 */ 

iAIDA::AIDA_XMLStore::AIDA_StoreXML::AIDA_StoreXML( const std::string& name,
						     bool readOnly,
						     bool createNew,
						     const std::string& options):
  m_name( name ), 
  m_backingStore( 0 ),
  m_readOnly( readOnly)
{

  // set plugins to be used 
  tuplePluginType = defaultTuplePluginType;   
  histoPluginType = defaultHistoPluginType;
  dpsPluginType = defaultDPSPluginType;
  funcPluginType = defaultFuncPluginType;


  // parse option ( default is now compress files) 
  m_compressLevel = ZIPCOMPRESSION; 
  if (!options.empty() ) { 

    // transform to lower case string
    std::string opt = options; 
    std::transform( opt.begin(), opt.end(), opt.begin(), tolower); 

    if (opt.find("uncompress") != std::string::npos ) {
       m_compressLevel = UNCOMPRESSION; 
    }
    if (opt.find("bzip") != std::string::npos ) {
       m_compressLevel = BZIPCOMPRESSION; 
    }
    if (opt.find("gzip") != std::string::npos ) {
       m_compressLevel = GZIPCOMPRESSION; 
    }
  }
  // std::cout << "++> request for compression level " << m_compressLevel << std::endl;

  // read all object from file 
  // if writing mode and creating new no need to read ! 
  if (readOnly || !createNew) 
    readAllObjects();
}


iAIDA::AIDA_XMLStore::AIDA_StoreXML::~AIDA_StoreXML()
{
  close(); 
  if ( m_backingStore ) delete m_backingStore;
}


const std::string&
iAIDA::AIDA_XMLStore::AIDA_StoreXML::name() const
{
  return m_name;
}


bool
iAIDA::AIDA_XMLStore::AIDA_StoreXML::writeObject( const AIDA::IManagedObject& dataObject,
						   const std::string& path )
{

  /// some checks at the beginning 

  if ( m_objectTypes.find( path ) != m_objectTypes.end() ) return false;
  AIDA::Dev::IDevManagedObject* object =
    dynamic_cast< AIDA::Dev::IDevManagedObject* >( &( const_cast<AIDA::IManagedObject&>( dataObject ) ) );
  if ( ! object ) return false;

  const std::string& type = object->userLevelClassType();
  /* 
  if ( ! iAIDA::AIDA_XMLStore::SupportedAIDATypes::supportedTypes().isTypeSupported( type ) ) {
    return false;
  }
  */
  //object->setUpToDate( false );

  m_objectTypes[ path ] = type;
  m_objectRefs[ path ] = object;

  /* 
  if ( m_objectsToDelete.find( path ) != m_objectsToDelete.end() ) {
    m_objectsToDelete.erase( path );
    m_objectsToUpdate.insert( path );
  }
  else {
    m_objectsToAdd.insert( path );
  }
  */

  return true; 
}


/// Here we need the AIDA types

AIDA::IManagedObject*
iAIDA::AIDA_XMLStore::AIDA_StoreXML::copyAndWrite( const AIDA::IManagedObject& dataObject,
							  const std::string& path )
{
  // check first if original object exists
  if ( m_objectTypes.find( path ) != m_objectTypes.end() ) return 0;

  // Identify the type of the new object

  const AIDA::Dev::IDevManagedObject& object = dynamic_cast< const AIDA::Dev::IDevManagedObject& >( dataObject );
  const std::string& type = object.userLevelClassType();

  // Create the actual copy
  AIDA::Dev::IDevManagedObject* newObject = 0;

  if ( type == "IHistogram1D" || type == "IHistogram2D" || type == "IHistogram3D" ||
       type == "IProfile1D" || type == "IProfile2D" ||
       type == "ICloud1D" || type == "ICloud2D" || type == "ICloud3D" ) {

    /*
    // load the histogram plugin and create a  dev histo fractory
    std::auto_ptr<AIDA_Plugin::AIDA_PluginType>   plugin( pl.load(histoPluginType) ) ; 
    if ( ! plugin.get() ) {
      XML_STORE_REPORT_ERROR( "The xml store could not load a plugin of type " + histoPluginType );
      return 0; 
    }
    std::auto_ptr<AIDA::Dev::IDevHistogramFactory> hf(plugin->createDevHistogramFactory() ); 
    if ( ! hf.get() ) {
      XML_STORE_REPORT_ERROR( "The xml store could not create a dev histogram factory" );
      return 0; 
    }
    */
    std::auto_ptr<AIDA::Dev::IDevHistogramFactory> hf( new AIDA_Histogram_native::AIDA_DevHistogramFactory() ); 

    if ( type == "IHistogram1D" ) {
      newObject = hf->createCopy( dynamic_cast<const AIDA::IHistogram1D&>( dataObject ) );
    }
    else if ( type == "IHistogram2D" ) {
      newObject = hf->createCopy( dynamic_cast<const AIDA::IHistogram2D&>( dataObject ) );
    }
    else if ( type == "IHistogram3D" ) {
      newObject = hf->createCopy( dynamic_cast<const AIDA::IHistogram3D&>( dataObject ) );
    }
    else if ( type == "IProfile1D" ) {
      newObject = hf->createCopy( dynamic_cast<const AIDA::IProfile1D&>( dataObject ) );
    }
    else if ( type == "IProfile2D" ) {
      newObject = hf->createCopy( dynamic_cast<const AIDA::IProfile2D&>( dataObject ) );
    }
    else if ( type == "ICloud1D" ) {
      newObject = hf->createCopy( dynamic_cast<const AIDA::ICloud1D&>( dataObject ) );
    }
    else if ( type == "ICloud2D" ) {
      newObject = hf->createCopy( dynamic_cast<const AIDA::ICloud2D&>( dataObject ) );
    }
    else if ( type == "ICloud3D" ) {
      newObject = hf->createCopy( dynamic_cast<const AIDA::ICloud3D&>( dataObject ) );
    }
  }
  else if ( type == "IDataPointSet" ) {  // DataPointSet cases

    /*    
    // load the dps plugin 
    std::auto_ptr<AIDA_Plugin::AIDA_PluginType>   plugin( pl.load(dpsPluginType) ) ; 
    if ( ! plugin.get() ) {
      XML_STORE_REPORT_ERROR( "The xml store could not load a plugin of type " + dpsPluginType );
      return 0; 
    }
    std::auto_ptr<AIDA::Dev::IDevDataPointSetFactory> df(plugin->createDevDataPointSetFactory() ); 
    if ( ! df.get() ) {
      XML_STORE_REPORT_ERROR( "The xml store could not create a dev data point set  factory" );
      return 0; 
    }
    */
    std::auto_ptr<AIDA::Dev::IDevDataPointSetFactory> df(new AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory() ); 

    newObject = df->createCopy( dynamic_cast<const AIDA::IDataPointSet &>( dataObject ) );
  }
  else if ( type == "IFunction" ) {  // function cases

    /*
    // load the function plugin 
    std::auto_ptr<AIDA_Plugin::AIDA_PluginType>   plugin( pl.load(funcPluginType) ) ; 
    if ( ! plugin.get() ) {
      XML_STORE_REPORT_ERROR( "The xml store could not load a plugin of type " + funcPluginType );
      return 0; 
    }
    std::auto_ptr<AIDA::Dev::IDevFunctionFactory> ff(plugin->createDevFunctionFactory() ); 
    if ( ! ff.get() ) {
      XML_STORE_REPORT_ERROR( "The xml store could not create a dev function factory" );
      return 0; 
    }
    */
    std::auto_ptr<AIDA::Dev::IDevFunctionFactory> ff( new AIDA_Function::AIDA_DevFunctionFactory() ); 

    AIDA::IManagedObject * mo = const_cast<AIDA::IManagedObject *> (&dataObject); 
    newObject = ff->cloneFunction( dynamic_cast<AIDA::IFunction *>( mo) );
  }

  // Others to be added...

  // Identify the name
  unsigned int pos = path.size();
  for ( int iChar = path.size() - 1; iChar >= 0; --iChar ) {
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
iAIDA::AIDA_XMLStore::AIDA_StoreXML::retrieveObject( const std::string & path )
{
  // object should be in memory... 
  std::map< std::string, AIDA::Dev::IDevManagedObject* >::iterator iObjectRef = m_objectRefs.find( path );
  if ( iObjectRef == m_objectRefs.end() ) return 0;
  if ( iObjectRef->second != 0 ) return iObjectRef->second;
  // object not found 
  return 0; 
}


bool
iAIDA::AIDA_XMLStore::AIDA_StoreXML::removeObject( const std::string& path )
{
  // just erase object from maps 
  if ( m_objectTypes.find( path ) == m_objectTypes.end() ) return false;
  m_objectTypes.erase( path );
  m_objectRefs.erase( path );
  return true; 
}

// here is path or also changing object name ?? 

bool
iAIDA::AIDA_XMLStore::AIDA_StoreXML::moveObject(const std::string& from, const std::string& to )
{
  // erase old object from maps and re-insert with new path 

  std::map< std::string, std::string >::const_iterator sourceType = m_objectTypes.find( from );
  if ( sourceType == m_objectTypes.end() ) return false;

  std::map< std::string, AIDA::Dev::IDevManagedObject *  >::const_iterator sourceRef = m_objectRefs.find( from );
  if ( sourceRef == m_objectRefs.end() ) return false;


  std::string type = sourceType->second;
  // check if can  move ITuple
  if ( type == "ITuple" && !canMoveTuples() ) return false;
 

  AIDA::Dev::IDevManagedObject* object = sourceRef->second; 
  m_objectTypes.erase( from );
  m_objectRefs.erase( from );
  m_objectTypes.insert( std::make_pair( to, type ) );
  m_objectRefs.insert( std::make_pair( to, object ) );
  return true;
}


std::vector< std::string >
iAIDA::AIDA_XMLStore::AIDA_StoreXML::listObjectPaths( const std::string directory,
						       bool recursive ) const

  // This is just a exact copy from Memory Store 
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


std::vector< std::string >
iAIDA::AIDA_XMLStore::AIDA_StoreXML::listObjectTypes( const std::string directory,
						       bool recursive ) const
{

  // This is exact copy from memory store 

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
bool iAIDA::AIDA_XMLStore::AIDA_StoreXML::canMoveTuples() const {return false;}
bool iAIDA::AIDA_XMLStore::AIDA_StoreXML::canCopyTuples() const {return false;}


// specific tuples methods (all copied from AIDA_MemoryStore.cpp ) 

bool
iAIDA::AIDA_XMLStore::AIDA_StoreXML::writeTupleHeader( AIDA::Dev::ITupleHeader& header )
{
  if ( ! m_backingStore)  createBackingStore(); 
  if ( ! m_backingStore ) return false;
  else return m_backingStore->writeTupleHeader( header );
}


bool
iAIDA::AIDA_XMLStore::AIDA_StoreXML::readTupleHeader( AIDA::Dev::ITupleHeader& header )
{
  if ( ! m_backingStore)  createBackingStore(); 
  if ( ! m_backingStore ) return false;
  else return m_backingStore->readTupleHeader( header );
}


bool
iAIDA::AIDA_XMLStore::AIDA_StoreXML::bindVariable( AIDA::Dev::ITupleHeader& header, int variableIndex )
{
  if ( ! m_backingStore)  createBackingStore(); 
  if ( ! m_backingStore ) return false;
  else return m_backingStore->bindVariable( header, variableIndex );
}


void*
iAIDA::AIDA_XMLStore::AIDA_StoreXML::variableAddress( AIDA::Dev::ITupleHeader& header, int variableIndex )
{
  if ( ! m_backingStore)  createBackingStore(); 
  if ( ! m_backingStore ) return 0;
  else return m_backingStore->variableAddress( header, variableIndex );
}


const void*
iAIDA::AIDA_XMLStore::AIDA_StoreXML::variableAddress( const AIDA::Dev::ITupleHeader& header, int variableIndex ) const
{
  if ( ! m_backingStore ) return 0;
  else return m_backingStore->variableAddress( header, variableIndex );
} 


bool
iAIDA::AIDA_XMLStore::AIDA_StoreXML::clearBindings( const AIDA::Dev::ITupleHeader& header )
{
  if ( ! m_backingStore)  createBackingStore(); 
  if ( ! m_backingStore ) return false;
  else return m_backingStore->clearBindings( header );
}


bool
iAIDA::AIDA_XMLStore::AIDA_StoreXML::writeTupleRow( AIDA::Dev::ITupleHeader& header )
{
  if ( ! m_backingStore)  createBackingStore(); 
  if ( ! m_backingStore ) return false;
  else return m_backingStore->writeTupleRow( header );
}


bool
iAIDA::AIDA_XMLStore::AIDA_StoreXML::readTupleRow( AIDA::Dev::ITupleHeader& header )
{
  if ( ! m_backingStore)  createBackingStore(); 
  if ( ! m_backingStore ) return false;
  else return m_backingStore->readTupleRow( header );
}


bool
iAIDA::AIDA_XMLStore::AIDA_StoreXML::resetTuple( AIDA::Dev::ITupleHeader& header )
{
  if ( ! m_backingStore)  createBackingStore(); 
  if ( ! m_backingStore ) return false;
  else return m_backingStore->resetTuple( header );
}

  /// create Managed object from XML and return also the path of object from the file  

 AIDA::Dev::IDevManagedObject * 
   iAIDA::AIDA_XMLStore::AIDA_StoreXML::createFromXML(const DataXML::DataObject * xmlObj, std::string & path ) { 


   // get type and then translate according the object
   std::string type = xmlObj->name();
   std::string name, dirPath; 
   AIDA::Dev::IDevManagedObject* newObject = 0;

   /*
   // get plugin loader
   AIDA_Plugin::AIDA_PluginLoader & pl = AIDA_Plugin::AIDA_PluginManager::instance();
   */

   // case of object need to be created by the histogram factory
   // (histogram or profile or cloud ) 
   if (type.find("histogram") != std::string::npos  || 
       type.find("profile") != std::string::npos || 
       type.find("cloud") != std::string::npos ) { 
     /*
     // load the histogram plugin and create a  dev histo fractory
     std::auto_ptr<AIDA_Plugin::AIDA_PluginType>   plugin( pl.load(histoPluginType) ) ; 
     if ( ! plugin.get() ) {
       XML_STORE_REPORT_ERROR( "The xml store could not load a plugin of type " + histoPluginType );
       return 0; 
     }
     std::auto_ptr<AIDA::Dev::IDevHistogramFactory> hf(plugin->createDevHistogramFactory() ); 
     if ( ! hf.get() ) {
       XML_STORE_REPORT_ERROR( "The xml store could not create a dev histogram factory" );
       return 0; 
     }
     */
     std::auto_ptr<AIDA::Dev::IDevHistogramFactory> hf( new AIDA_Histogram_native::AIDA_DevHistogramFactory() ); 

     // create now according to specif type 
     if (type == "histogram1d") { 
       iAIDA::AIDA_XMLStore::Histo1DTranslator ht(xmlObj); 
       newObject = ht.createFromXML(*hf); 
       name = ht.name(); 
       dirPath = ht.path();   // in XML is saved directory path 
     }
     if (type == "histogram2d") { 
       iAIDA::AIDA_XMLStore::Histo2DTranslator ht(xmlObj); 
       newObject = ht.createFromXML(*hf); 
       name = ht.name(); 
       dirPath = ht.path();   // in XML is saved directory path 
     }
     if (type == "histogram3d") { 
       iAIDA::AIDA_XMLStore::Histo3DTranslator ht(xmlObj); 
       newObject = ht.createFromXML(*hf); 
       name = ht.name(); 
       dirPath = ht.path();   // in XML is saved directory path 
     }
     if (type == "cloud1d") { 
       iAIDA::AIDA_XMLStore::Cloud1DTranslator ct(xmlObj); 
       newObject = ct.createFromXML(*hf); 
       name = ct.name(); 
       dirPath = ct.path();   // in XML is saved directory path 
     }
     if (type == "cloud2d") { 
       iAIDA::AIDA_XMLStore::Cloud2DTranslator ct(xmlObj); 
       newObject = ct.createFromXML(*hf); 
       name = ct.name(); 
       dirPath = ct.path();   // in XML is saved directory path 
     }
     if (type == "cloud3d") { 
       iAIDA::AIDA_XMLStore::Cloud3DTranslator ct(xmlObj); 
       newObject = ct.createFromXML(*hf); 
       name = ct.name(); 
       dirPath = ct.path();   // in XML is saved directory path 
     }
     if (type == "profile1d") { 
       iAIDA::AIDA_XMLStore::Profile1DTranslator t(xmlObj); 
       newObject = t.createFromXML(*hf); 
       name = t.name(); 
       dirPath = t.path();   // in XML is saved directory path 
     }
     if (type == "profile2d") { 
       iAIDA::AIDA_XMLStore::Profile2DTranslator t(xmlObj); 
       newObject = t.createFromXML(*hf); 
       name = t.name(); 
       dirPath = t.path();   // in XML is saved directory path 
     }
   }
   else if (type == "dataPointSet") { 

     /*
     // load the dps plugin 
     std::auto_ptr<AIDA_Plugin::AIDA_PluginType>   plugin( pl.load(dpsPluginType) ) ; 
     if ( ! plugin.get() ) {
       XML_STORE_REPORT_ERROR( "The xml store could not load a plugin of type " + dpsPluginType );
       return 0; 
     }
     std::auto_ptr<AIDA::Dev::IDevDataPointSetFactory> df(plugin->createDevDataPointSetFactory() ); 
     if ( ! df.get() ) {
       XML_STORE_REPORT_ERROR( "The xml store could not create a dev data point set  factory" );
       return 0; 
     }
     */
     std::auto_ptr<AIDA::Dev::IDevDataPointSetFactory> df(new AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory() ); 

     // translate the object from XML
     iAIDA::AIDA_XMLStore::DataPointSetTranslator t(xmlObj); 
     newObject = t.createFromXML(*df); 
     name = t.name(); 
     dirPath = t.path();   // in XML is saved directory path 
   }

   else if (type == "function") { 

     /*
     // load the function plugin 
     std::auto_ptr<AIDA_Plugin::AIDA_PluginType>   plugin( pl.load(funcPluginType) ) ; 
     if ( ! plugin.get() ) {
       XML_STORE_REPORT_ERROR( "The xml store could not load a plugin of type " + funcPluginType );
       return 0; 
     }
     std::auto_ptr<AIDA::Dev::IDevFunctionFactory> ff(plugin->createDevFunctionFactory() ); 
     if ( ! ff.get() ) {
       XML_STORE_REPORT_ERROR( "The xml store could not create a dev function factory" );
       return 0; 
     }
     */
     std::auto_ptr<AIDA::Dev::IDevFunctionFactory> ff( new AIDA_Function::AIDA_DevFunctionFactory() ); 

     // translate the object from XML
     iAIDA::AIDA_XMLStore::FunctionTranslator t(xmlObj); 
     newObject = t.createFromXML(*ff); 
     name = t.name(); 
     dirPath = t.path();   // in XML is saved directory path 
   }

   else if (type == "tuple" ) { 

     /*
     // load the tuple plugin 
     std::auto_ptr<AIDA_Plugin::AIDA_PluginType>   plugin( pl.load(tuplePluginType) ) ; 
     if ( ! plugin.get() ) {
       XML_STORE_REPORT_ERROR( "The xml store could not load a plugin of type " + tuplePluginType );
       return 0; 
     }
     std::auto_ptr<AIDA::Dev::IDevTupleFactory> tf(plugin->createDevTupleFactory() ); 
     if ( ! tf.get() ) {
       XML_STORE_REPORT_ERROR( "The xml store could not create a dev tuple factory" );
       return 0; 
     }
     */
     std::auto_ptr<AIDA::Dev::IDevTupleFactory> tf( new AIDA_Tuple_native::AIDA_DevTupleFactory() ); 

     // translate the object from XML
     iAIDA::AIDA_XMLStore::TupleTranslator t(xmlObj); 
     if ( ! m_backingStore)  createBackingStore(); 
     newObject = t.createFromXML(*tf,m_backingStore); 
     name = t.name(); 
     dirPath = t.path();   // in XML is saved directory path 

   }


   // set name for new object 
   if ( newObject ) {

     // path here is defined as "dirPath + name" 
     if (dirPath == "/") 
       path = dirPath + name; 
     else { 
       // clean up of slahses at the end of dirPath
       if (dirPath[dirPath.size()-1] == '/' ) 
 	dirPath = dirPath.substr(0,dirPath.size()-1); 
       path = dirPath + "/" + name; 
     //path = dirPath; 
     }

     m_objectRefs[path] = newObject;
     newObject->setUpToDate( true );
     newObject->setName( name );
   }

   return newObject;

 }

  /// append Managed object to XML stream 
  /// but not write yet to the file   

 bool  iAIDA::AIDA_XMLStore::AIDA_StoreXML::writeToXML(AIDA::Dev::IDevManagedObject * object, const std::string & path ) { 

   if (!m_outputTranslator) return false; 

   // get type 
   const std::string&  type = object->userLevelClassType();
   std::string name = object->name(); 
   
   // extract directory path from full path 
   std::vector<std::string> pathItems;
   boost::split( pathItems, path, boost::is_any_of( "/" ), boost::token_compress_on);
   
   // check for first "/" staring from the end 
   std::string dirPath = ""; 
   std::string lastItem = pathItems[pathItems.size()-1]; // last item of path
   for (size_t index = 0; index < pathItems.size()-2; index++) {
      dirPath += pathItems[index];
   }

   std::cout << "writeToXML> found path, dirPath, lastItem, name " 
             << "'" << path     << "'" << " "
             << "'" << dirPath  << "'" << " "
             << "'" << lastItem << "'" << " " 
             << "'" << name     << "'" << " " 
             << std::endl;

/* old 
   int iiii = 0; 
   for ( int iChar = path.size()-1; iChar >= 0; --iChar ) {
     const char& c = path[iChar];
     if ( c == '/' ) {
       if (iChar > 0) 
 	       dirPath = path.substr(0,iChar); 
       else 
 	       dirPath = path.substr(0,1);    // case of "/" 

       if (iChar < static_cast<int>(path.size()-1) ) 
 	       lastItem = path.substr(iChar+1,path.size()); 
       iiii = iChar;
       break;     
     }
   }
*/

   if (lastItem != name) {
   //    std::cerr << "Path and name: " << lastItem << " n= " << name << " path = " << path << std::endl; 
 //     XML_STORE_REPORT_ERROR( "AIDA_XMLStore:WritetoXML : Warning path is uncompatible with name" );
 //     return false;
     // avoid to write empty names
     if (name == "") name = lastItem;
   }

   //  std::string::size_type idx = path.rfind(name);
   //if ( idx != std::string::npos && idx == path.length() - name.length() ) 

   /* not working   
   if (path.substr(path.rfind(name)) == name) 
     // build dir eliminating extra "/" at the end 
     dirPath = path.substr(0,path.length()-name.length()-1);   			*/

   if ( type == "IHistogram1D" ) {
     AIDA::IHistogram1D* p = dynamic_cast< AIDA::IHistogram1D* >( object );
     if ( !p ) return false;   
      // append object to store (but not write, that is done at commit time)
     if (!m_outputTranslator->append(*p,name,dirPath)) return false; 
   }
   if ( type == "IHistogram2D" ) {
     AIDA::IHistogram2D* p = dynamic_cast< AIDA::IHistogram2D* >( object );
     if ( !p ) return false;   
      // append object to store (but not write, that is done at commit time)
     if (!m_outputTranslator->append(*p,name,dirPath)) return false; 
   }
   if ( type == "IHistogram3D" ) {
     AIDA::IHistogram3D* p = dynamic_cast< AIDA::IHistogram3D* >( object );
     if ( !p ) return false;   
      // append object to store (but not write, that is done at commit time)
     if (!m_outputTranslator->append(*p,name,dirPath)) return false; 
   }
   if ( type == "ICloud1D" ) {
     AIDA::ICloud1D* p = dynamic_cast< AIDA::ICloud1D* >( object );
     if ( !p ) return false;   
      // append object to store (but not write, that is done at commit time)
     if (!m_outputTranslator->append(*p,name,dirPath)) return false; 
   }
   if ( type == "ICloud2D" ) {
     AIDA::ICloud2D* p = dynamic_cast< AIDA::ICloud2D* >( object );
     if ( !p ) return false;   
      // append object to store (but not write, that is done at commit time)
     if (!m_outputTranslator->append(*p,name,dirPath)) return false; 
   }
   if ( type == "ICloud3D" ) {
     AIDA::ICloud3D* p = dynamic_cast< AIDA::ICloud3D* >( object );
     if ( !p ) return false;   
      // append object to store (but not write, that is done at commit time)
     if (!m_outputTranslator->append(*p,name,dirPath)) return false; 
   }
   if ( type == "IProfile1D" ) {
     AIDA::IProfile1D* p = dynamic_cast< AIDA::IProfile1D* >( object );
     if ( !p ) return false;   
      // append object to store (but not write, that is done at commit time)
     if (!m_outputTranslator->append(*p,name,dirPath)) return false; 
   }
   if ( type == "IProfile2D" ) {
     AIDA::IProfile2D* p = dynamic_cast< AIDA::IProfile2D* >( object );
     if ( !p ) return false;   
      // append object to store (but not write, that is done at commit time)
     if (!m_outputTranslator->append(*p,name,dirPath)) return false; 
   }
   if ( type == "IDataPointSet" ) {
     AIDA::IDataPointSet* p = dynamic_cast< AIDA::IDataPointSet* >( object );
     if ( !p ) return false;   
      // append object to store (but not write, that is done at commit time)
     if (!m_outputTranslator->append(*p,name,dirPath)) return false; 
   }
   if ( type == "IFunction" ) {
     AIDA::IFunction* f = dynamic_cast< AIDA::IFunction* >( object );
     if ( !f ) return false;   
      // append object to store (but not write, that is done at commit time)
     if (!m_outputTranslator->append(*f,name,dirPath)) return false; 
   }
   if ( type == "ITuple" ) {
     AIDA::ITuple* t = dynamic_cast< AIDA::ITuple* >( object );
     if ( !t ) return false;   
      // append object to store (but not write, that is done at commit time)
     if (!m_outputTranslator->append(*t,name,dirPath)) return false; 
   }

   return true; 
 }


 void iAIDA::AIDA_XMLStore::AIDA_StoreXML::createBackingStore() { 
   m_backingStore = new iAIDA::AIDA_MemoryStore::MemoryBackingStore( tuplePluginType );

 }

// ----- actual I/O follows here -----

bool
iAIDA::AIDA_XMLStore::AIDA_StoreXML::commit()
{

   // nothing to be done if we're readonly ... 
   if (m_readOnly) return true; 


  // here I write XML store 
  // create output translator ( a new stream is created for every commit)
  // create output stream 

  std::ostream * stream = 0;

  bool m_compress = m_compressLevel;

  namespace bio = boost::iostreams;

  if (m_compress) { 
     if  (m_compressLevel == GZIPCOMPRESSION) { 
         bio::filtering_ostream * out = new bio::filtering_ostream();
         out->push(bio::gzip_compressor());
         out->push(bio::file_sink(m_name));
         stream = out;
         if ( !stream ) std::cout << "ERROR !!! ";
         std::cout << "++> created gzip compressed boost_iostream" << std::endl;
     } else if  (m_compressLevel == BZIPCOMPRESSION) { 
         bio::filtering_ostream * out = new bio::filtering_ostream();
         out->push(bio::bzip2_compressor());
         out->push(bio::file_sink(m_name));     
         stream = out;
         if ( !stream ) std::cout << "ERROR !!! ";
         std::cout << "++> created bzip2 compressed boost_iostream" << std::endl;
     } else {  // default ZIPCOMPRESSION for backward compatibility
        stream = new ogzstream(m_name.c_str());    
        std::cout << "++> created ogzstream" << std::endl;
     }
  } 
  else { 
    stream = new std::ofstream(m_name.c_str());
  }

  if (stream == 0) { 
    XML_STORE_REPORT_ERROR(" Error opening file " + m_name ); 
    return false; 
  }
  m_outputTranslator = new iAIDA::AIDA_XMLStore::StoreTranslator(*stream);

  writeAll();
  
  stream->flush(); // maybe only for boost iostreams ???
   
  delete m_outputTranslator;  

  delete stream; 

  return true; 
}

bool  
iAIDA::AIDA_XMLStore::AIDA_StoreXML::writeAll() { 

    for ( std::map< std::string, AIDA::Dev::IDevManagedObject* >::iterator iObj = m_objectRefs.begin(); iObj != m_objectRefs.end(); ++iObj ) {
      AIDA::Dev::IDevManagedObject* object = iObj->second;
      // why sometimes I have an empty objects entered ???
      if (object) { 
	      std::string path = iObj->first;
	      writeToXML(object, path);
      } 
    }
    // flush all to the file 
    if (!m_outputTranslator->write()) { 
      XML_STORE_REPORT_ERROR(" Error writing to the file " + m_name ); 
      return false; 
    }

    return true;     
}


bool
iAIDA::AIDA_XMLStore::AIDA_StoreXML::close()
{
  // no operation - file is close at every commit
  return true;
}



// read all objects from the file and load then in memory creating new Managed 
// Objects

bool
iAIDA::AIDA_XMLStore::AIDA_StoreXML::readAllObjects( )
{
  
  iAIDA::AIDA_XMLStore::StoreTranslator * translator = 0; 

  bool m_compress = m_compressLevel;

  namespace bio = boost::iostreams;

  std::istream * zinputStream = 0; 

  if (m_compress) { 
     if  (m_compressLevel == GZIPCOMPRESSION) { 
         bio::filtering_istream * in = new bio::filtering_istream();
         in->push(bio::gzip_decompressor());
         in->push(bio::file_source(m_name));
         zinputStream = in;
      } else if (m_compressLevel == BZIPCOMPRESSION) { 
         bio::filtering_istream * in = new bio::filtering_istream();
         in->push(bio::bzip2_decompressor());
         in->push(bio::file_source(m_name));
         zinputStream = in;
     } else {
        zinputStream = new igzstream(m_name.c_str() );
     }
  } else { 
    zinputStream = new std::ifstream(m_name.c_str()); 
  }

  if (zinputStream == 0 ) { 
    XML_STORE_REPORT_ERROR(" Error opening file " + m_name ); 
    return false; 
  }
  // check also the state
  if (! *zinputStream ) { 
    XML_STORE_REPORT_ERROR(" Error opening file " + m_name ); 
    return false; 
  }
  translator = new iAIDA::AIDA_XMLStore::StoreTranslator(*zinputStream); 

  //-ap #endif


  // get the root element of the store parent to all objects (aida element) 
  DataXML::DataObject rootElement; 
  if (!translator->read(rootElement)) return false; 
 
  const std::vector<DataXML::DataObject> elements = rootElement.children();

  // check if any object is preset
  if (elements.size() == 0) return false; 
  
  // loop on all elements and build map of types and objects
  for (std::vector<DataXML::DataObject>::const_iterator iObj = elements.begin(); iObj != elements.end(); ++iObj) { 

    std::string type =  iObj->name();
    std::string path; 
    AIDA::Dev::IDevManagedObject * newObj = createFromXML( &(*iObj), path ); 
    // fill maps 
    if (newObj) { 
      m_objectTypes.insert( std::make_pair( path, newObj->userLevelClassType() ) );
      m_objectRefs.insert( std::make_pair( path, newObj ) );
    }
  }


  // delete the translator since is not needed anymore
  delete translator; 
  //delete inputStream; 
  delete zinputStream;
  return true; 
}


