#include "AIDA_StoreRoot.h"
#include "AIDA_RootStoreHelper.h"
#include "RootPathFormater.h"
#include "AIDA_ROOT/HistoConverter.h"
#include "SupportedAIDATypes.h"
#include "RootTuple.h"
#include "RootTree.h"

#include "TFile.h"
#include "TKey.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TNtuple.h"
#include "TLeaf.h"

#include "AIDA_Dev/IDevManagedObject.h"
#include "AIDA_Dev/ITupleFactoryProvider.h"
#include "AIDA_Dev/IDevHistogram1D.h"
#include "AIDA_Dev/IDevHistogram2D.h"
#include "AIDA_Dev/IDevHistogram3D.h"
#include "AIDA_Dev/IDevProfile1D.h"
#include "AIDA_Dev/IDevProfile2D.h"

#include "AIDA_Dev/IDevTupleFactory.h"
#include "AIDA_Dev/IDevTuple.h"
#include "AIDA_Dev/ITupleHeader.h"
#include "AIDA_Dev/ITupleVariableDescription.h"
#include "AIDA_Dev/ITupleVariableStatistics.h"

#include <typeinfo>
#include <set>
#include "AIDA_Dev/IDevHistogramFactory.h"
//#include "AIDA_MemoryStore/MemoryBackingStore.h"

// #include "AIDA_Plugin/AIDA_PluginLoader.h"
// #include "AIDA_Plugin/AIDA_PluginType.h"
#include "AIDA_Histogram_native/AIDA_DevHistogramFactory.h"
#include "AIDA_Tuple_native/AIDA_DevTupleFactory.h"

#include <memory>
#include <fstream>
#include <iostream>
#include <algorithm>

#ifdef ROOT_STORE_NO_EXCEPTIONS_TO_USER
#include <iostream>
#define ROOT_STORE_REPORT_ERROR( XXX ) std::cerr << XXX << std::endl
#else
#include <stdexcept>
#define ROOT_STORE_REPORT_ERROR( XXX ) throw std::runtime_error( XXX )
#endif

static const std::string float_type = "float";
static const int defaultTupleBufferSize = 32000; 

static std::string defaultHistoPluginType = "AIDA_Histogram_Root"; 
static std::string defaultTuplePluginType = "AIDA_Tuple_Native"; 

//static std::string defaultHistoPluginType = "AIDA_Histogram_Native"; 




/* create the Root store
 *   - read always all the object - if file does not exists nothing 
 *       will be found 
 *   - always write again in a new file at commit all the objects    
 *     so createNew has no influence
 */ 

pi::AIDA_RootStore::AIDA_StoreRoot::AIDA_StoreRoot( const std::string& name,
						     bool readOnly,
						     bool createNew,
						     const std::string& options):
  m_name( name ), 
  m_readMode(false)
{

   // set lugins to be used 

  histoPluginType = defaultHistoPluginType;
  tuplePluginType = defaultTuplePluginType;


 // create root file representing  the store

  std::string fileOption = "UPDATE"; 
  if (createNew) 
    fileOption = "RECREATE"; 

  if (readOnly) { 
    fileOption = "READ";
    m_readMode = true; 
  }

  // compression
  int icompress = 1;
  // parse option ( default is now compress files) 
  if (!options.empty() ) { 
    if (options.find("uncompress") != std::string::npos )
      icompress = 0; 
  }
  // create the store
  m_store = new TFile(name.c_str(),fileOption.c_str(),name.c_str(),icompress);

  if (m_store == 0 ) { 
    ROOT_STORE_REPORT_ERROR(" Error opening file " + name ); 
    return; 
  }


  // if tuple need to create here backing store (see XML store) 



  // read all object from file 
  // if writing mode and creating new no need to read ! 
  if (readOnly || !createNew) 
    collectObjectTypes( );
}


pi::AIDA_RootStore::AIDA_StoreRoot::~AIDA_StoreRoot()
{
  close(); 
  delete m_store; 
}


const std::string&
pi::AIDA_RootStore::AIDA_StoreRoot::name() const
{
  return m_name;
}


bool
pi::AIDA_RootStore::AIDA_StoreRoot::writeObject( const AIDA::IManagedObject& dataObject,
						   const std::string& path )
{

  /// some checks at the beginning 

  if ( m_objectTypes.find( path ) != m_objectTypes.end() ) return false;
  AIDA::Dev::IDevManagedObject* object =
    dynamic_cast< AIDA::Dev::IDevManagedObject* >( &( const_cast<AIDA::IManagedObject&>( dataObject ) ) );
  if ( ! object ) return false;

  
  const std::string& type = object->userLevelClassType();
   
  if ( ! pi::AIDA_RootStore::SupportedAIDATypes::supportedTypes().isTypeSupported( type ) ) {
    return false;
  }
  
  object->setUpToDate( false );


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


/// Here we need the AIDA types

AIDA::IManagedObject*
pi::AIDA_RootStore::AIDA_StoreRoot::copyAndWrite( const AIDA::IManagedObject& dataObject,
							  const std::string& path )
{
  // check first if original object exists
  if ( m_objectTypes.find( path ) != m_objectTypes.end() ) return 0;

  // Identify the type of the new object

  const AIDA::Dev::IDevManagedObject& object = dynamic_cast< const AIDA::Dev::IDevManagedObject& >( dataObject );
  const std::string& type = object.userLevelClassType();
  if ( ! pi::AIDA_RootStore::SupportedAIDATypes::supportedTypes().isTypeSupported( type ) ) {
    return false;
  }


//-ap   // get plugin loader
//-ap   AIDA_Plugin::AIDA_PluginLoader & pl = AIDA_Plugin::AIDA_PluginManager::instance();


  // Create the actual copy
  AIDA::Dev::IDevManagedObject* newObject = 0;

  if ( type == "IHistogram1D" || type == "IHistogram2D" || type == "IHistogram3D" ||
       type == "IProfile1D" || type == "IProfile2D" ||
       type == "ICloud1D" || type == "ICloud2D" || type == "ICloud3D" ) {

//-ap     // load the histogram plugin and create a  dev histo fractory
//-ap     std::auto_ptr<AIDA_Plugin::AIDA_PluginType>   plugin( pl.load(histoPluginType) ) ; 
//-ap     if ( ! plugin.get() ) {
//-ap       ROOT_STORE_REPORT_ERROR( "The root store could not load a plugin of type " + histoPluginType );
//-ap       return 0; 
//-ap     }
//-ap     std::auto_ptr<AIDA::Dev::IDevHistogramFactory> hf(plugin->createDevHistogramFactory() ); 
//-ap     if ( ! hf.get() ) {
//-ap       ROOT_STORE_REPORT_ERROR( "The root store could not create a dev histogram factory" );
//-ap       return 0; 
//-ap     }

    std::auto_ptr<AIDA::Dev::IDevHistogramFactory> hf(new iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory() ); 
    if ( ! hf.get() ) {
      ROOT_STORE_REPORT_ERROR( "The root store could not create a dev histogram factory" );
      return 0; 
    }

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
    
  }


  // Others to be added...

  // Identify the name
  unsigned int pos = path.size();
  for (  int iChar = path.size() - 1; iChar >= 0; --iChar ) {
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
pi::AIDA_RootStore::AIDA_StoreRoot::retrieveObject( const std::string & path )
{
  // object is in  in memory... 
  std::map< std::string, AIDA::Dev::IDevManagedObject* >::iterator iObjectRef = m_objectRefs.find( path );
  if ( iObjectRef == m_objectRefs.end() ) return 0;
  if ( iObjectRef->second != 0 ) return iObjectRef->second;
  

  // fetch object from the store 
  // get ID and directory ? 
  
//-ap   // fetch  plugin loader
//-ap   AIDA_Plugin::AIDA_PluginLoader & pl = AIDA_Plugin::AIDA_PluginManager::instance();

 
  // get type and then translate according to the object

  AIDA::Dev::IDevManagedObject* newObject = 0;
  const std::string& type = m_objectTypes[ iObjectRef->first ];

  // extract directory name from full path 
  std::pair<std::string, std::string> nameAndDir = getDirectoryAndName( path) ; 
  std::string dirPath = nameAndDir.first; 
  std::string name =  nameAndDir.second; 


  // if object is a Tuple 
  // special case : use then readTupleHeader() 

  if ( type == "ITuple") { 
 
//-ap     // load the tuple plugin 
//-ap     std::auto_ptr<AIDA_Plugin::AIDA_PluginType>   plugin( pl.load(tuplePluginType) ) ; 
//-ap     if ( ! plugin.get() ) {
//-ap       ROOT_STORE_REPORT_ERROR( "The ROOT store could not load a plugin of type " + tuplePluginType );
//-ap       return 0; 
//-ap     }
//-ap     std::auto_ptr<AIDA::Dev::IDevTupleFactory> tf(plugin->createDevTupleFactory() ); 
//-ap     if ( ! tf.get() ) {
//-ap       ROOT_STORE_REPORT_ERROR( "The ROOT store could not create a dev tuple factory" );
//-ap       return 0; 
//-ap     }

    std::auto_ptr<AIDA::Dev::IDevTupleFactory> tf( new iAIDA::AIDA_Tuple_native::AIDA_DevTupleFactory() ); 
    if ( ! tf.get() ) {
      ROOT_STORE_REPORT_ERROR( "The ROOT store could not create a dev tuple factory" );
      return 0; 
    }

    AIDA::Dev::IDevTuple* tuple = tf->create( this, "" );
    if ( ! tuple ) {
      ROOT_STORE_REPORT_ERROR( "Could not create a tuple" );
      return 0;
    }
    tuple->header().setPathInStore( path );
    m_objectRefs[path] = tuple;
    if ( ! this->readTupleHeader( tuple->header() ) ) {
      m_objectRefs[path] = 0;
      ROOT_STORE_REPORT_ERROR( "Could not read the header of the tuple from the store" );
      return 0;
    }
    else 
      newObject = tuple;
  }

  // other cases (histogram , dps , etc..

  else { 


    // go to directory where to fetch object     
    std::string dirRoot = m_name + ":" + dirPath; 
    bool dirExist = m_store->cd( dirRoot.c_str() ); 
    if (!dirExist) {
      ROOT_STORE_REPORT_ERROR( "The Store could not cd to " + dirPath); 
      return 0; 
    } 
        
    // get TObject from the store
    std::string rootPath = m_name + ":" + path; 
    //std::cout << "getting ROOT object from path " << path << std::endl; 
  
    TObject * tobj = m_store->Get(rootPath.c_str()); 
    if ( !tobj ) { 
      ROOT_STORE_REPORT_ERROR( "The Store could not retrieve " + path); 
      return 0; 
    }
  

    // case of object need to be created by the histogram factory
    // (histogram or profile or cloud ) 
    if (type.find("Histogram") != std::string::npos  || 
	type.find("Profile") != std::string::npos || 
	type.find("Cloud") != std::string::npos ) { 

      // fetch a dev histogram factory 

//-ap       // load the histogram plugin and create a  dev histo fractory
//-ap       std::auto_ptr<AIDA_Plugin::AIDA_PluginType>   plugin( pl.load(histoPluginType) ) ; 
//-ap       if ( ! plugin.get() ) {
//-ap 	ROOT_STORE_REPORT_ERROR( "The root store could not load a plugin of type " + histoPluginType );
//-ap 	return 0; 
//-ap       }
//-ap       std::auto_ptr<AIDA::Dev::IDevHistogramFactory> hf(plugin->createDevHistogramFactory() ); 
//-ap       if ( ! hf.get() ) {
//-ap 	ROOT_STORE_REPORT_ERROR( "The root store could not create a dev histogram factory" );
//-ap 	return 0; 
//-ap       }

      std::auto_ptr<AIDA::Dev::IDevHistogramFactory> hf( new iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory() ); 
      if ( ! hf.get() ) {
	ROOT_STORE_REPORT_ERROR( "The root store could not create a dev histogram factory" );
	return 0; 
      }


      // create now according to specif type 
      if (type == "IHistogram1D") { 
	TH1 * thist = dynamic_cast<TH1 *>(tobj); 
	if (!thist) return 0;
	newObject = AIDA_RootConverter::HistoConverter::copyToAida(*hf,*thist); 
      }
      if (type == "IHistogram2D") { 
	TH2 * thist = dynamic_cast<TH2 *>(tobj); 
	if (!thist) return 0;
	newObject = AIDA_RootConverter::HistoConverter::copyToAida(*hf,*thist);
      }
      if (type == "IHistogram3D") { 
	TH3 * thist = dynamic_cast<TH3 *>(tobj); 
	if (!thist) return 0;
	newObject = AIDA_RootConverter::HistoConverter::copyToAida(*hf,*thist);
      }
      if (type == "IProfile1D") { 
	TProfile * tprof = dynamic_cast<TProfile *>(tobj); 
	if (!tprof) return 0;
	newObject = AIDA_RootConverter::HistoConverter::copyToAida(*hf,*tprof);
      }
      if (type == "IProfile2D") { 
	TProfile2D * tprof = dynamic_cast<TProfile2D *>(tobj); 
	if (!tprof) return 0;
	newObject = AIDA_RootConverter::HistoConverter::copyToAida(*hf,*tprof);
      }
    }

  }  // endif on type 
    
  // for all types: register the new object   
  if ( newObject ) {
    newObject->setUpToDate( true );
    newObject->setName( name );
    //std::cout << "retrieved object " << type << " with name " << name << std::endl; 
  }
  return newObject;
}


bool
pi::AIDA_RootStore::AIDA_StoreRoot::removeObject( const std::string& path )
{
  // just erase object from maps 
  if ( m_objectTypes.find( path ) == m_objectTypes.end() ) return false;
  m_objectTypes.erase( path );
  m_objectRefs.erase( path );
  return true; 
}

// here is path or also changing object name ?? 

bool
pi::AIDA_RootStore::AIDA_StoreRoot::moveObject(const std::string& from, const std::string& to )
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
pi::AIDA_RootStore::AIDA_StoreRoot::listObjectPaths( const std::string directory,
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
pi::AIDA_RootStore::AIDA_StoreRoot::listObjectTypes( const std::string directory,
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


bool
pi::AIDA_RootStore::AIDA_StoreRoot::commit()
{
  // here I write Root store 
  
  // no action in readmode
  if (m_readMode) return true; 

  // if empty return 
  if (m_objectRefs.begin() == m_objectRefs.end() ) return true; 

  for ( std::map< std::string, AIDA::Dev::IDevManagedObject* >::iterator iObj = m_objectRefs.begin(); iObj != m_objectRefs.end(); ++iObj ) {
    AIDA::Dev::IDevManagedObject* object = iObj->second;
    // why sometimes I have an empty objects entered ???
    if (object) { 
      std::string path = iObj->first;
      writeToRoot(object,path);

      object->setUpToDate( true );
    } 
  }
  // flush all to the file   
  if (!m_store->Write()) { 
    ROOT_STORE_REPORT_ERROR(" Error writing to the file " + m_name ); 
    return false; 
  }

  // for debugging
  //m_store->ls(); 

  return true; 
}


bool
pi::AIDA_RootStore::AIDA_StoreRoot::close()
{
  // before closing the file I need to delete the object I copied ??
  if (m_readMode) return true; 

  // close file
  m_store->Close();
  return true;
}


bool pi::AIDA_RootStore::AIDA_StoreRoot::canMoveTuples() const {return false;}
bool pi::AIDA_RootStore::AIDA_StoreRoot::canCopyTuples() const {return false;}


// specific tuples methods (all copied from AIDA_MemoryStore.cpp ) 

bool
pi::AIDA_RootStore::AIDA_StoreRoot::writeTupleHeader( AIDA::Dev::ITupleHeader& header )
{
  // fetch variables names and types 
  const std::string& path = header.pathInStore();
  // if ntuple already exists return
  if ( m_tuples.find( path ) != m_tuples.end() ) return false;

  if ( path[0] != '/' ) {
    ROOT_STORE_REPORT_ERROR( "Wrong path in store " + path);     
    return false; 
  }

  // find name and directory 
  std::string name =  getDirectoryAndName( path).second;

  // move to subdirectory (create if not exists)
  if (!setCurrentDirectory( path) )  return false;  

  bool noFloat = false;
  bool unsupportedType = false; 
  std::vector< std::string > variableNames;
  std::vector< std::string > variableTypes;
  for ( int iVar = 0; iVar < header.numberOfVariables(); ++iVar ) {
    const AIDA::Dev::ITupleVariableDescription* description = header.variableDescription( iVar );
    variableNames.push_back( description->variableName() );
    variableTypes.push_back( description->variableType() );
    if ( variableTypes.back() != float_type ) noFloat = true;
    if ( ! pi::AIDA_RootStore::RootTree::isTypeSupported(  variableTypes.back() ) ) unsupportedType = true; 
  }
  if ( variableNames.size() == 0 ) { 
    ROOT_STORE_REPORT_ERROR("Tuple contains an empty list of variables"); 
    return false;
  }
  
  std::string title =  dynamic_cast< AIDA::ITuple* >( m_objectRefs[path] )->title(); 
  // 
  if ( !noFloat ) {
  
   
    // support only ROOT TNtuple now 

    // to be implemented: get buffer size from input 
    
    //int bufSize = m_optionParser->bufferSizeForRWN(); 
    int bufSize = defaultTupleBufferSize; 
    pi::AIDA_RootStore::IPersistentTuple * rootTuple = new pi::AIDA_RootStore::RootTuple(name, title, variableNames, bufSize, m_store ); 
    m_tuples.insert( std::make_pair( path, rootTuple) ); 
    return true;
  }

  else { 
    if (unsupportedType) { 
    // support for simple tuples 
      ROOT_STORE_REPORT_ERROR("Tuple based on columns which contains types not yet supported"); 
      return false;
    }
    // simple tuples 
    pi::AIDA_RootStore::IPersistentTuple * rootTree = new pi::AIDA_RootStore::RootTree(name, title, variableNames, variableTypes, m_store ); 
    m_tuples.insert( std::make_pair( path, rootTree) ); 
    return true;
  }

}


bool
pi::AIDA_RootStore::AIDA_StoreRoot::readTupleHeader( AIDA::Dev::ITupleHeader& header )
{

  const std::string& path = header.pathInStore();
  // if alread exists return 
  if ( m_tuples.find( path ) != m_tuples.end() ) return true;

  // path[0] should be "/"
  if ( path[0] != '/' ) {
    ROOT_STORE_REPORT_ERROR( "Wrong path in store " + path);     
    return false; 
  }



  // extract directory name from full path 

  std::pair<std::string, std::string> nameAndDir = getDirectoryAndName( path) ; 
  std::string dirPath = nameAndDir.first; 
  std::string name =  nameAndDir.second; 
    
  std::string dirRoot = m_name + ":" + dirPath; 
  bool dirExist = m_store->cd( dirRoot.c_str() ); 
  if (!dirExist) {
    ROOT_STORE_REPORT_ERROR( "The Store could not cd to " + dirPath); 
    return false; 
  } 
    
    
  // get TObject from the store
  std::string rootPath = m_name + ":" + path; 
  //std::cout << "getting Tuple object " << name << " from path " << path << std::endl; 

  TObject * tobj = m_store->Get(rootPath.c_str()); 
  if ( !tobj ) { 
    ROOT_STORE_REPORT_ERROR( "The Store could not retrieve Object  " + path); 
    return false; 
  }
  // support root Tuples as TTree objects 
  TTree * ttree = dynamic_cast<TTree *> (tobj); 
  if (! ttree) { 
    ROOT_STORE_REPORT_ERROR( "The Store could not retrieve TTree  " + path); 
    return false; 
  }
  

   
  /* 
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
  */

//-ap   // get plugin loader
//-ap   AIDA_Plugin::AIDA_PluginLoader & pl = AIDA_Plugin::AIDA_PluginManager::instance();
//-ap 
//-ap   // load the tuple plugin 
//-ap   std::auto_ptr<AIDA_Plugin::AIDA_PluginType>   plugin( pl.load(tuplePluginType) ) ; 
//-ap   if ( ! plugin.get() ) {
//-ap     ROOT_STORE_REPORT_ERROR( "The Root store could not load a plugin of type " + tuplePluginType );
//-ap     return 0; 
//-ap   }
//-ap   std::auto_ptr<AIDA::Dev::IDevTupleFactory> tf(plugin->createDevTupleFactory() ); 
//-ap   if ( ! tf.get() ) {
//-ap       ROOT_STORE_REPORT_ERROR( "The Root store could not create a dev tuple factory" );
//-ap       return 0; 
//-ap   }

  std::auto_ptr<AIDA::Dev::IDevTupleFactory> tf( new iAIDA::AIDA_Tuple_native::AIDA_DevTupleFactory() ); 
  if ( ! tf.get() ) {
      ROOT_STORE_REPORT_ERROR( "The Root store could not create a dev tuple factory" );
      return 0; 
  }

  /*
    pi::AIDA_RootStore::Root::changeDirectory( m_cache->directoryInMemory( dirAndId.first ) );
    pi::AIDA_RootStore::Root::changeDirectory( m_cache->directoryInFile( dirAndId.first ) );
    pi::AIDA_RootStore::Root::loadObjectIntoMemory( dirAndId.second );
    pi::AIDA_RootStore::Root::changeDirectory( m_cache->directoryInMemory( dirAndId.first ) );
  */

  // create wrapper around root tree
  pi::AIDA_RootStore::IPersistentTuple * rootTree = 0;
  // case of tuple or tree 
  TNtuple * tntuple = dynamic_cast<TNtuple *> ( ttree); 
  // case is a TNtuple 
  if (tntuple)  
    rootTree = new  pi::AIDA_RootStore::RootTuple( tntuple); 
  else 
    rootTree = new pi::AIDA_RootStore::RootTree( ttree);

  // insert in the map
  m_tuples.insert( std::make_pair( path, rootTree ) ); 

  
  // Retrieve the title and the number of rows
  // why root has entries in a tree as double ? 
  const int numberOfEntries = static_cast<int>(ttree->GetEntries() );
  std::string title = ttree->GetTitle(); 

  // set title 
  dynamic_cast< AIDA::Dev::IDevTuple* >( m_objectRefs[ path ] )->setTitle( title );
  header.setNumberOfRows( numberOfEntries );
  header.setCurrentRowNumber( -1 );
  
  // Build the descriptions
  
  // get names and types 
  
  const std::vector<std::string> & variableNames = rootTree->variableNames(); 
  const std::vector<std::string> & variableTypes = rootTree->variableTypes(); 
  // root does not keep min and max so put +/- inf
  double COLMIN = -1.7976931348623157e+308;
  double COLMAX = -1.7976931348623157e+308;
  for ( unsigned int i = 0; i < variableNames.size(); ++i ) {
    AIDA::Dev::ITupleVariableDescription * description = tf->createDescription();
    description->setVariableName( variableNames[i] );
    description->setVariableType( variableTypes[i] );
    description->statistics().setStatistics( COLMIN, COLMAX, 0, 0, numberOfEntries); 
    header.setVariableDescription( description, true );
  }

    // not yet supported 
    /* 
       pi::AIDA_RootStore::CWNtuple* cwntuple = new pi::AIDA_RootStore::CWNtuple( m_cache->directoryInMemory( dirAndId.first ),
       dirAndId.second );
       cwntuple->readDescription( header, *tf, this );
       m_tuples.insert( std::make_pair( pathInStore, static_cast<pi::AIDA_RootStore::IPersistentTuple*>( cwntuple ) ) );
       return true;
    */ 
  return true;         
}



bool
pi::AIDA_RootStore::AIDA_StoreRoot::bindVariable( AIDA::Dev::ITupleHeader& header, int variableIndex )
{
  const std::string& pathInStore = header.pathInStore();
  if  ( pathInStore[0] == '/' ) {
    std::map< std::string, pi::AIDA_RootStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( pathInStore );
    if ( iTuple == m_tuples.end() ) return false;
    else return iTuple->second->bindVariable( variableIndex );
  }
  else {
    const std::string path = pathInStore.substr( pathInStore.find( "/" ) );
    std::map< std::string, pi::AIDA_RootStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( path );
    if ( iTuple == m_tuples.end() ) return false;
    else return iTuple->second->bindVariable( variableIndex, pathInStore );
  }
}


void*
pi::AIDA_RootStore::AIDA_StoreRoot::variableAddress( AIDA::Dev::ITupleHeader& header, int variableIndex )
{
  const std::string& pathInStore = header.pathInStore();
  if ( pathInStore[0] == '/' ) {
    std::map< std::string, pi::AIDA_RootStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( pathInStore );
    if ( iTuple == m_tuples.end() ) return 0;
    else return iTuple->second->variableAddress( variableIndex );
  }
  else {
    const std::string path = pathInStore.substr( pathInStore.find( "/" ) );
    std::map< std::string, pi::AIDA_RootStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( path );
    if ( iTuple == m_tuples.end() ) return 0;
    else {
      void * p = iTuple->second->variableAddress( variableIndex, pathInStore );
      return p;
    }
  }
}


void
pi::AIDA_RootStore::AIDA_StoreRoot::setVariableAddress(const  AIDA::Dev::ITupleHeader& header, int variableIndex, void * p )
{
  const std::string& pathInStore = header.pathInStore();
  if ( pathInStore[0] == '/' ) {
    std::map< std::string, pi::AIDA_RootStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( pathInStore );
    if ( iTuple == m_tuples.end() ) { 
      return;
    }
    else { 
      iTuple->second->setVariableAddress( variableIndex, p );
      return; 
    }
  }
  else {
    const std::string path = pathInStore.substr( pathInStore.find( "/" ) );
    std::map< std::string, pi::AIDA_RootStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( path );
    if ( iTuple == m_tuples.end() ) return;
    else {
      iTuple->second->setVariableAddress( variableIndex, pathInStore, p );
   }
  }
  return;
}


const void*
pi::AIDA_RootStore::AIDA_StoreRoot::variableAddress( const AIDA::Dev::ITupleHeader& header, int variableIndex ) const
{
  const std::string& pathInStore = header.pathInStore();
  if ( pathInStore[0] == '/' ) {
    std::map< std::string, pi::AIDA_RootStore::IPersistentTuple* >::const_iterator iTuple = m_tuples.find( pathInStore );
    if ( iTuple == m_tuples.end() ) return 0;
    else return iTuple->second->variableAddress( variableIndex );
  }
  else {
    const std::string path = pathInStore.substr( pathInStore.find( "/" ) );
    std::map< std::string, pi::AIDA_RootStore::IPersistentTuple* >::const_iterator iTuple = m_tuples.find( path );
    if ( iTuple == m_tuples.end() ) return 0;
    else  {
      const void * p = iTuple->second->variableAddress( variableIndex, pathInStore );
      return p;
    }
  }
} 


bool
pi::AIDA_RootStore::AIDA_StoreRoot::clearBindings( const AIDA::Dev::ITupleHeader& header )
{
  const std::string& pathInStore = header.pathInStore();
  if ( pathInStore[0] == '/' ) {
    std::map< std::string, pi::AIDA_RootStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( pathInStore );
    if ( iTuple == m_tuples.end() ) return false;
    else return iTuple->second->clearBindings();
  }
  else {
    const std::string path = pathInStore.substr( pathInStore.find( "/" ) );
    std::map< std::string, pi::AIDA_RootStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( path );
    if ( iTuple == m_tuples.end() ) return false;
    else return iTuple->second->clearBindings( pathInStore );
  }
}


bool
pi::AIDA_RootStore::AIDA_StoreRoot::writeTupleRow( AIDA::Dev::ITupleHeader& header )
{
  const std::string& pathInStore = header.pathInStore();
  if ( pathInStore[0] == '/' ) {
    std::map< std::string, pi::AIDA_RootStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( pathInStore );
    if ( iTuple == m_tuples.end() ) return false;
    else return iTuple->second->writeTupleRow( header.currentRowNumber() );
  }
  else {
    const std::string path = pathInStore.substr( pathInStore.find( "/" ) );
    std::map< std::string, pi::AIDA_RootStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( path );
    if ( iTuple == m_tuples.end() ) return false;
    else return iTuple->second->writeTupleRow( header.currentRowNumber(), pathInStore );
  }
}


bool
pi::AIDA_RootStore::AIDA_StoreRoot::readTupleRow( AIDA::Dev::ITupleHeader& header )
{
  const std::string& pathInStore = header.pathInStore();
  if ( pathInStore[0] == '/' ) {
    std::map< std::string, pi::AIDA_RootStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( pathInStore );
    if ( iTuple == m_tuples.end() ) return false;
    else return iTuple->second->readTupleRow( header.currentRowNumber() );
  }
  else {
    const std::string path = pathInStore.substr( pathInStore.find( "/" ) );
    std::map< std::string, pi::AIDA_RootStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( path );
    if ( iTuple == m_tuples.end() ) return false;
    else return iTuple->second->readTupleRow( header.currentRowNumber(), pathInStore );
  }
}


bool
pi::AIDA_RootStore::AIDA_StoreRoot::resetTuple( AIDA::Dev::ITupleHeader& header )
{
  const std::string& pathInStore = header.pathInStore();
  if ( pathInStore[0] == '/' ) {
    std::map< std::string, pi::AIDA_RootStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( pathInStore );
    if ( iTuple == m_tuples.end() ) return false;
    else return iTuple->second->reset();
  }
  else {
    const std::string path = pathInStore.substr( pathInStore.find( "/" ) );
    std::map< std::string, pi::AIDA_RootStore::IPersistentTuple* >::iterator iTuple = m_tuples.find( path );
    if ( iTuple == m_tuples.end() ) return false;
    else return iTuple->second->reset( pathInStore );
  }
}


 /// append Managed object to Root file
 /// but not write yet to the file  just convert objects 
 
bool  pi::AIDA_RootStore::AIDA_StoreRoot::writeToRoot(AIDA::Dev::IDevManagedObject * object, const std::string & path ) { 




  // get type 
  const std::string&  type = object->userLevelClassType();
  std::string name = object->name(); 

  // move to subdirectory (create if not exists
  if (!setCurrentDirectory( path) )  return false;  

  if (name != getDirectoryAndName(path).second) {
    ROOT_STORE_REPORT_ERROR( "AIDA_RootStore:WritetoRoot : Warning path is uncompatible with name ");
    return false;
  }
  
  //  std::string::size_type idx = path.rfind(name);
  //if ( idx != std::string::npos && idx == path.length() - name.length() ) 
  
  /* not working   
  if (path.substr(path.rfind(name)) == name) 
    // build dir eliminating extra "/" at the end 
    dirPath = path.substr(0,path.length()-name.length()-1);   			*/
  
  // append object to store (but not write, that is done at commit time)
  TH1 * rObj = 0; 
  if ( type == "IHistogram1D" ) {
    AIDA::IHistogram1D* p = dynamic_cast< AIDA::IHistogram1D* >( object );
    if ( !p ) return false;   
    rObj =  AIDA_RootConverter::HistoConverter::copyToRoot (*p,name);
  }
  if ( type == "IHistogram2D" ) {
    AIDA::IHistogram2D* p = dynamic_cast< AIDA::IHistogram2D* >( object );
    if ( !p ) return false;   
    rObj =  AIDA_RootConverter::HistoConverter::copyToRoot (*p,name); 
 }
  if ( type == "IHistogram3D" ) {
    AIDA::IHistogram3D* p = dynamic_cast< AIDA::IHistogram3D* >( object );
    if ( !p ) return false;   
    rObj =  AIDA_RootConverter::HistoConverter::copyToRoot (*p,name); 
  }
  if ( type == "IProfile1D" ) {
    AIDA::IProfile1D* p = dynamic_cast< AIDA::IProfile1D* >( object );
    if ( !p ) return false;   
    rObj = AIDA_RootConverter::HistoConverter::copyToRoot (*p,name); 
  }
  if ( type == "IProfile2D" ) {
    AIDA::IProfile2D* p = dynamic_cast< AIDA::IProfile2D* >( object );
    if ( !p ) return false;   
    rObj = AIDA_RootConverter::HistoConverter::copyToRoot (*p,name); 
  }
  // attach object to the store
  if (rObj) rObj->SetDirectory(m_store);

  /*
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
  */

  return true; 
}

// collect all types of objects and name from root file
void
pi::AIDA_RootStore::AIDA_StoreRoot::collectObjectTypes(const std::string & directory )
{  
  AIDA::Dev::IDevManagedObject * dummyObject = 0;
  std::string rootDir = m_name + ":" + directory;
  bool dirExist = m_store->cd(rootDir.c_str() ); 
  if (!dirExist) {  
    ROOT_STORE_REPORT_ERROR(" Error changing to directory " + rootDir); 
    return; 
  }
  // read all keys in the current Directory 
  TIter nextkey(gDirectory->GetListOfKeys()); 
  std::string curDir = directory; 
  TKey * key; 
  while ( (key = (TKey*)nextkey())) {
    std::string type(key->GetClassName() ); 
    std::string name(key->GetName() );
    std::string path = curDir + "/" + name;
    //std::cout << "location " << curDir << "  :   " << name << "  " << type << std::endl; 
    if (type != "TDirectory") { 
      m_objectTypes.insert(std::make_pair( path, AIDA_RootStore_Helper::aidaType(type) ) );
      m_objectRefs.insert(std::make_pair(path, dummyObject) ); 
    }
    else { 
      // be RECURSIVE HERE
      collectObjectTypes(path); 
    }			   

  }
}


std::pair<std::string, std::string> 
pi::AIDA_RootStore::AIDA_StoreRoot::getDirectoryAndName(const std::string & path) {  
  
  std::pair<std::string, std::string> dirAndName; 
  std::vector<std::string> dirList = pi::AIDA_RootStore::RootPathFormater::theFormater().formPathNames( path ); 
  // if path list is empty return
  if (dirList.size() == 0) return dirAndName; 
  // name of object is last item 
  std::string name = dirList.back();
  
  std::string dirPath = "";
  if (dirList.size() == 0) dirPath = "/"; 
  // loop down the directories 
  for (unsigned int i = 0; i < dirList.size()-1; ++i)   
    dirPath += "/" + dirList[i]; 

  dirAndName = std::make_pair(dirPath, name); 
  return dirAndName; 
}
    

bool 
pi::AIDA_RootStore::AIDA_StoreRoot::setCurrentDirectory(const std::string & path) {

  std::vector<std::string> dirList = pi::AIDA_RootStore::RootPathFormater::theFormater().formPathNames( path ); 
  // if path is list is empty return
  if (dirList.size() == 0) return false; 
  std::string lastItem = dirList.back();

  dirList.erase(dirList.end()-1); 
  TDirectory * subDir = m_store; 
  std::string dirPath = "";
  if (dirList.size() == 0) dirPath = "/"; 
  // loop down the directories 
  for (unsigned int i = 0; i < dirList.size(); ++i) {  
    dirPath += "/" + dirList[i]; 
    std::string dirRoot = m_name + ":" + dirPath; 
    // look if subdir exists
    bool dirExist = false; 
    TKey * key = subDir->FindKey(dirList[i].c_str() ); 
    if ( key != 0) {  
      std::string type(key->GetClassName() );       
      if  ( type == "TDirectory") {  
	dirExist = subDir->cd( dirRoot.c_str() ); 
      }

    }
    
    // if not existing create the subdirectory
    if (! dirExist) { 
      TDirectory *  newDir = subDir->mkdir( dirList[i].c_str() ); 
      if (newDir == 0) { 
	ROOT_STORE_REPORT_ERROR(" Error creating subDirectory " + dirPath); 
	return false; 
      }
      dirExist = newDir->cd(); 
      //newDir->pwd();
      if (!dirExist) { 
	ROOT_STORE_REPORT_ERROR(" Error going to subDirectory " + dirPath); 
	return false; 
      }
    }
    // set subDir to current directory 
    //gDirectory->pwd();
    subDir = gDirectory; 
  }
  return true;
} 
