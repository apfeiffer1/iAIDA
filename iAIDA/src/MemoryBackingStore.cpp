#include "AIDA_MemoryStore/MemoryBackingStore.h"
#include "AIDA_Dev/IDevTupleFactory.h"
#include "AIDA_Dev/IDevTuple.h"
#include "AIDA_Dev/ITupleHeader.h"
#include "AIDA_Dev/ITupleVariableDescription.h"
#include "AIDA_Dev/ITupleVariableStatistics.h"
#include "AIDA/IAnnotation.h"

#include "AIDA_Tuple_native/AIDA_DevTupleFactory.h"

//-ap #include "AIDA_Plugin/AIDA_PluginLoader.h"
//-ap #include "AIDA_Plugin/AIDA_PluginType.h"

#include <set>
#include <stdexcept>


static std::string defaultTuplePluginType = "AIDA_Tuple_Native"; 
// needed since AIDA 3.2
static std::string titleKey="Title";


namespace iAIDA {
  namespace AIDA_MemoryStore {

class MemoryStoreSupportedTypes
{
public:
  MemoryStoreSupportedTypes()
  {
    m_supportedTypes.insert( "double" );
    m_supportedTypes.insert( "float" );
    m_supportedTypes.insert( "long" );
    m_supportedTypes.insert( "int" );
    m_supportedTypes.insert( "short" );
    m_supportedTypes.insert( "char" );
    m_supportedTypes.insert( "bool" );
    m_supportedTypes.insert( "std::string" );
    m_supportedTypes.insert( "AIDA::ITuple" );
  }
  ~MemoryStoreSupportedTypes()
  {  }
  bool isTypeSupported( const std::string& type ) const
  {
    return ( m_supportedTypes.find( type ) != m_supportedTypes.end() );
  }
private:
  std::set< std::string > m_supportedTypes;
};

  }
}

static const iAIDA::AIDA_MemoryStore::MemoryStoreSupportedTypes supportedTypes;


iAIDA::AIDA_MemoryStore::MemoryBackingStore::MemoryBackingStore(std::string tuplePluginType):
  m_factory( 0)
{
  /*
  if ( tuplePluginType.empty( ) ) tuplePluginType = defaultTuplePluginType;  
  // create backing store (need tuple factory so fetch it using plugin) 
  // need to load a tuple implementation (use plugin manager) 
  AIDA_Plugin::AIDA_PluginLoader & pl = AIDA_Plugin::AIDA_PluginManager::instance();
  // load the plugin 
  std::auto_ptr<AIDA_Plugin::AIDA_PluginType>   plugin( pl.load(tuplePluginType) ) ; 
  if ( plugin.get() ) {
    m_factory = plugin->createDevTupleFactory();
  }
  else { 
    throw std::runtime_error( "Error creating Tuples plugin - cannot create Memory Backing store");
  }
  */
  m_factory = new AIDA_Tuple_native::AIDA_DevTupleFactory();
}


iAIDA::AIDA_MemoryStore::MemoryBackingStore::~MemoryBackingStore()
{
  m_tuples.clear();
  for ( std::map< std::string, iAIDA::AIDA_MemoryStore::MemoryTupleData* >::iterator i = m_tupleData.begin();
	i != m_tupleData.end(); ++i ) {
    delete i->second;
  }
  m_tupleData.clear();

  if (m_factory) delete m_factory;
}


bool
iAIDA::AIDA_MemoryStore::MemoryBackingStore::writeTupleHeader( AIDA::Dev::ITupleHeader& header )
{
  const std::string& localPath = header.pathInStore();
  if ( m_tupleData.find( localPath ) != m_tupleData.end() ) return false;
  iAIDA::AIDA_MemoryStore::MemoryTupleData* tuple = new iAIDA::AIDA_MemoryStore::MemoryTupleData;
  for ( int i = 0; i < header.numberOfVariables(); ++i ) {
    AIDA::Dev::ITupleVariableDescription* description = header.variableDescription( i );
    const std::string& type = description->variableType();
    if ( ! supportedTypes.isTypeSupported( type ) ) return false;
    if (! m_factory) return false; 
    tuple->variables().push_back( m_factory->createDescription( *description ) );
    if ( type == "double" ) tuple->doubleVariableData().insert( std::make_pair( i, std::vector< double >() ) );
    else if ( type == "float" ) tuple->floatVariableData().insert( std::make_pair( i, std::vector< float >() ) );
    else if ( type == "long" ) tuple->longVariableData().insert( std::make_pair( i, std::vector< long >() ) );
    else if ( type == "int" ) tuple->intVariableData().insert( std::make_pair( i, std::vector< int >() ) );
    else if ( type == "short" ) tuple->shortVariableData().insert( std::make_pair( i, std::vector< short >() ) );
    else if ( type == "char" ) tuple->charVariableData().insert( std::make_pair( i, std::vector< char >() ) );
    else if ( type == "bool" ) tuple->boolVariableData().insert( std::make_pair( i, std::vector< bool >() ) );
    else if ( type == "std::string" ) tuple->stringVariableData().insert( std::make_pair( i, std::vector< std::string >() ) );
    else if ( type == "AIDA::ITuple" ) tuple->tupleVariableData().insert( std::make_pair( i, std::vector< iAIDA::AIDA_MemoryStore::MemoryTupleData * >() ) );
  }
  tuple->title( header.annotation().value( titleKey ) );
  m_tupleData.insert( std::make_pair( localPath, tuple ) );
  m_tuples.insert( std::make_pair( localPath, iAIDA::AIDA_MemoryStore::MemoryPersistentTuple( static_cast<iAIDA::AIDA_MemoryStore::IMemoryBackingStore*>(this),
											       *m_factory,
											       tuple,
											       localPath ) ) );
  return true;
}

bool
iAIDA::AIDA_MemoryStore::MemoryBackingStore::readTupleHeader( AIDA::Dev::ITupleHeader& header )
{
  const std::string& localPath = header.pathInStore();
  std::map< std::string, iAIDA::AIDA_MemoryStore::MemoryTupleData* >::iterator iTupleData = m_tupleData.find( localPath );
  if ( iTupleData == m_tupleData.end() ) return false;
  iAIDA::AIDA_MemoryStore::MemoryTupleData* tuple = iTupleData->second;
  header.annotation().setValue( titleKey, tuple->title() );
  if ( m_tuples.find( localPath ) == m_tuples.end() ) {
    m_tuples.insert( std::make_pair( localPath,
				     iAIDA::AIDA_MemoryStore::MemoryPersistentTuple( static_cast<iAIDA::AIDA_MemoryStore::IMemoryBackingStore*>(this),
										      *m_factory,
										      tuple,
										      localPath ) ) );
  }

  for ( unsigned int i = 0; i < tuple->variables().size(); ++i ) {
    header.setVariableDescription( tuple->variables()[i], false );
  }
  // Get the number of rows and inform the tuple header
  unsigned int nRows = 0;
  if ( tuple->doubleVariableData().size() > 0 ) nRows = tuple->doubleVariableData().begin()->second.size();
  else if ( tuple->floatVariableData().size() > 0 ) nRows = tuple->floatVariableData().begin()->second.size();
  else if ( tuple->longVariableData().size() > 0 ) nRows = tuple->longVariableData().begin()->second.size();
  else if ( tuple->intVariableData().size() > 0 ) nRows = tuple->intVariableData().begin()->second.size();
  else if ( tuple->shortVariableData().size() > 0 ) nRows = tuple->shortVariableData().begin()->second.size();
  else if ( tuple->charVariableData().size() > 0 ) nRows = tuple->charVariableData().begin()->second.size();
  else if ( tuple->boolVariableData().size() > 0 ) nRows = tuple->boolVariableData().begin()->second.size();
  else if ( tuple->stringVariableData().size() > 0 ) nRows = tuple->stringVariableData().begin()->second.size();
  else if ( tuple->tupleVariableData().size() > 0 ) nRows = tuple->tupleVariableData().begin()->second.size();
  header.setNumberOfRows( static_cast< int >( nRows ) );
  header.setCurrentRowNumber( -1 );
  return true;
}

bool
iAIDA::AIDA_MemoryStore::MemoryBackingStore::removePersistentTuple( const std::string& path )
{
  std::map< std::string, iAIDA::AIDA_MemoryStore::MemoryPersistentTuple >::iterator iTuple = m_tuples.find( path );
  if ( iTuple == m_tuples.end() ) return false;
  m_tuples.erase( path );
  std::map< std::string, iAIDA::AIDA_MemoryStore::MemoryTupleData* >::iterator iTupleData = m_tupleData.find(path);
  if ( m_tupleData.find(path) != m_tupleData.end() ) {
    iAIDA::AIDA_MemoryStore::MemoryTupleData* p = iTupleData->second;
    if ( p ) delete p;
    m_tupleData.erase( path );
  }
  return true;
}

bool
iAIDA::AIDA_MemoryStore::MemoryBackingStore::setTupleData( const std::string& path, iAIDA::AIDA_MemoryStore::MemoryTupleData* data)
{
  if ( ! data ) return false;
  std::map< std::string, iAIDA::AIDA_MemoryStore::MemoryTupleData* >::iterator iTupleData = m_tupleData.find(path);
  if ( m_tupleData.find(path) != m_tupleData.end() ) {
    iTupleData->second = data;
    return true;
  }
  else return false;
}

iAIDA::AIDA_MemoryStore::IMemoryPersistentTuple*
iAIDA::AIDA_MemoryStore::MemoryBackingStore::findPersistentTuple( const std::string& path )
{
  std::map< std::string, iAIDA::AIDA_MemoryStore::MemoryPersistentTuple >::iterator iTuple = m_tuples.find( path );
  if ( iTuple == m_tuples.end() ) return 0;
  else return &( iTuple->second );
}


bool
iAIDA::AIDA_MemoryStore::MemoryBackingStore::bindVariable( AIDA::Dev::ITupleHeader& header, int variableIndex )
{
  const std::string& localPath = header.pathInStore();
  std::map< std::string, iAIDA::AIDA_MemoryStore::MemoryPersistentTuple >::iterator iTuple = m_tuples.find( localPath );
  if ( iTuple == m_tuples.end() ) return false;
  return iTuple->second.bindVariable( variableIndex );
}

void*
iAIDA::AIDA_MemoryStore::MemoryBackingStore::variableAddress( AIDA::Dev::ITupleHeader& header, int variableIndex )
{
  const std::string& localPath = header.pathInStore();
  std::map< std::string, iAIDA::AIDA_MemoryStore::MemoryPersistentTuple >::iterator iTuple = m_tuples.find( localPath );
  if ( iTuple == m_tuples.end() ) return 0;
  return iTuple->second.variableAddress( variableIndex );
}

const void*
iAIDA::AIDA_MemoryStore::MemoryBackingStore::variableAddress( const AIDA::Dev::ITupleHeader& header, int variableIndex ) const
{
  const std::string& localPath = header.pathInStore();
  std::map< std::string, iAIDA::AIDA_MemoryStore::MemoryPersistentTuple >::const_iterator iTuple = m_tuples.find( localPath );
  if ( iTuple == m_tuples.end() ) return 0;
  return iTuple->second.variableAddress( variableIndex );
}

/// set the address of a variable. Needed when type of object is not known 
/// not implemented for backing store

bool
iAIDA::AIDA_MemoryStore::MemoryBackingStore::clearBindings( const AIDA::Dev::ITupleHeader& header )
{
  const std::string& localPath = header.pathInStore();
  std::map< std::string, iAIDA::AIDA_MemoryStore::MemoryPersistentTuple >::iterator iTuple = m_tuples.find( localPath );
  if ( iTuple == m_tuples.end() ) return false;
  return iTuple->second.clearBindings();
}

bool
iAIDA::AIDA_MemoryStore::MemoryBackingStore::writeTupleRow( AIDA::Dev::ITupleHeader& header )
{
  const std::string& localPath = header.pathInStore();
  std::map< std::string, iAIDA::AIDA_MemoryStore::MemoryPersistentTuple >::iterator iTuple = m_tuples.find( localPath );
  if ( iTuple == m_tuples.end() ) return false;
  if ( ! ( iTuple->second.writeTupleRow( header.numberOfRows() - 1 ) ) ) return false;
  for ( int i = 0; i < header.numberOfVariables(); ++i ) {
    const AIDA::Dev::ITupleVariableStatistics& tupleStatistics = header.variableDescription( i )->statistics();
    AIDA::Dev::ITupleVariableStatistics& storeStatistics = ( iTuple->second.tupleData()->variables() )[i]->statistics();
    storeStatistics.setStatistics( tupleStatistics.min(),
				   tupleStatistics.max(),
				   tupleStatistics.mean(),
				   tupleStatistics.rms(),
				   tupleStatistics.entries() );
  }
  return true;
}

bool
iAIDA::AIDA_MemoryStore::MemoryBackingStore::readTupleRow( AIDA::Dev::ITupleHeader& header )
{
  const std::string& localPath = header.pathInStore();
  std::map< std::string, iAIDA::AIDA_MemoryStore::MemoryPersistentTuple >::iterator iTuple = m_tuples.find( localPath );
  if ( iTuple == m_tuples.end() ) return false;
  return iTuple->second.readTupleRow( header.currentRowNumber() );
}

bool
iAIDA::AIDA_MemoryStore::MemoryBackingStore::resetTuple( AIDA::Dev::ITupleHeader& header )
{
  const std::string& localPath = header.pathInStore();
  std::map< std::string, iAIDA::AIDA_MemoryStore::MemoryTupleData* >::iterator iTuple = m_tupleData.find( localPath );
  if ( iTuple == m_tupleData.end() ) return false;
  MemoryTupleData& tuple = *( iTuple->second );
  for( std::map< int, std::vector< double > >::iterator i = tuple.doubleVariableData().begin();
       i != tuple.doubleVariableData().end(); ++i ) i->second.clear();
  for( std::map< int, std::vector< float > >::iterator i = tuple.floatVariableData().begin();
       i != tuple.floatVariableData().end(); ++i ) i->second.clear();
  for( std::map< int, std::vector< long > >::iterator i = tuple.longVariableData().begin();
       i != tuple.longVariableData().end(); ++i ) i->second.clear();
  for( std::map< int, std::vector< int > >::iterator i = tuple.intVariableData().begin();
       i != tuple.intVariableData().end(); ++i ) i->second.clear();
  for( std::map< int, std::vector< short > >::iterator i = tuple.shortVariableData().begin();
       i != tuple.shortVariableData().end(); ++i ) i->second.clear();
  for( std::map< int, std::vector< char > >::iterator i = tuple.charVariableData().begin();
       i != tuple.charVariableData().end(); ++i ) i->second.clear();
  for( std::map< int, std::vector< bool > >::iterator i = tuple.boolVariableData().begin();
       i != tuple.boolVariableData().end(); ++i ) i->second.clear();
  for( std::map< int, std::vector< std::string > >::iterator i = tuple.stringVariableData().begin();
       i != tuple.stringVariableData().end(); ++i ) i->second.clear();
  for( std::map< int, std::vector< iAIDA::AIDA_MemoryStore::MemoryTupleData* > >::iterator i = tuple.tupleVariableData().begin();
       i != tuple.tupleVariableData().end(); ++i ) {
    for ( std::vector< iAIDA::AIDA_MemoryStore::MemoryTupleData* >::iterator j = i->second.begin(); j != i->second.end(); ++j ) {
      if ( *j ) delete *j;
    }
    i->second.clear();
  }
  return true;
}
