#include "DummyBackingStore.h"
#include "AIDA_Dev/IDevTupleFactory.h"
#include "AIDA_Dev/IDevTuple.h"
#include "AIDA_Dev/ITupleHeader.h"
#include "AIDA_Dev/ITupleVariableDescription.h"
#include "AIDA_Dev/ITupleVariableStatistics.h"
#include "AIDA/IAnnotation.h"
#include <set>

// needed since AIDA 3.2
static std::string titleKey="Title";


class DummyStoreSupportedTypes
{
public:
  DummyStoreSupportedTypes()
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
  ~DummyStoreSupportedTypes()
  {}
  bool isTypeSupported( const std::string& type ) const
  {
    return ( m_supportedTypes.find( type ) != m_supportedTypes.end() );
  }
private:
  std::set< std::string > m_supportedTypes;
};

static const DummyStoreSupportedTypes supportedTypes;


DummyBackingStore::DummyBackingStore( AIDA::Dev::IDevTupleFactory& factory ):
  m_factory( factory )
{}


DummyBackingStore::~DummyBackingStore()
{
  m_tuples.clear();
  for ( std::map< std::string, DummyTupleData* >::iterator i = m_tupleData.begin();
	i != m_tupleData.end(); ++i ) {
    delete i->second;
  }
  m_tupleData.clear();
}


bool
DummyBackingStore::writeTupleHeader( AIDA::Dev::ITupleHeader& header )
{
  const std::string& localPath = header.pathInStore();
  if ( m_tupleData.find( localPath ) != m_tupleData.end() ) return false;
  DummyTupleData* tuple = new DummyTupleData;
  for ( int i = 0; i < header.numberOfVariables(); ++i ) {
    AIDA::Dev::ITupleVariableDescription* description = header.variableDescription( i );
    const std::string& type = description->variableType();
    if ( ! supportedTypes.isTypeSupported( type ) ) return false;
    tuple->variables().push_back( m_factory.createDescription( *description ) );
    if ( type == "double" ) tuple->doubleVariableData().insert( std::make_pair( i, std::vector< double >() ) );
    else if ( type == "float" ) tuple->floatVariableData().insert( std::make_pair( i, std::vector< float >() ) );
    else if ( type == "long" ) tuple->longVariableData().insert( std::make_pair( i, std::vector< long >() ) );
    else if ( type == "int" ) tuple->intVariableData().insert( std::make_pair( i, std::vector< int >() ) );
    else if ( type == "short" ) tuple->shortVariableData().insert( std::make_pair( i, std::vector< short >() ) );
    else if ( type == "char" ) tuple->charVariableData().insert( std::make_pair( i, std::vector< char >() ) );
    else if ( type == "bool" ) tuple->boolVariableData().insert( std::make_pair( i, std::vector< bool >() ) );
    else if ( type == "std::string" ) tuple->stringVariableData().insert( std::make_pair( i, std::vector< std::string >() ) );
    else if ( type == "AIDA::ITuple" ) tuple->tupleVariableData().insert( std::make_pair( i, std::vector< DummyTupleData * >() ) );
  }
  tuple->title( header.annotation().value( titleKey ) );
  m_tupleData.insert( std::make_pair( localPath, tuple ) );
  m_tuples.insert( std::make_pair( localPath, DummyPersistentTuple( static_cast<IDummyBackingStore*>(this),
								    m_factory,
								    tuple,
								    localPath ) ) );
  return true;
}

bool
DummyBackingStore::readTupleHeader( AIDA::Dev::ITupleHeader& header )
{
  const std::string& localPath = header.pathInStore();
  std::map< std::string, DummyTupleData* >::iterator iTupleData = m_tupleData.find( localPath );
  if ( iTupleData == m_tupleData.end() ) return false;
  DummyTupleData* tuple = iTupleData->second;
  header.annotation().setValue( titleKey, tuple->title() );
  if ( m_tuples.find( localPath ) == m_tuples.end() ) {
    m_tuples.insert( std::make_pair( localPath, DummyPersistentTuple( static_cast<IDummyBackingStore*>(this), m_factory, tuple, localPath ) ) );
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
DummyBackingStore::removePersistentTuple( const std::string& path )
{
  std::map< std::string, DummyPersistentTuple >::iterator iTuple = m_tuples.find( path );
  if ( iTuple == m_tuples.end() ) return false;
  m_tuples.erase( path );
  std::map< std::string, DummyTupleData* >::iterator iTupleData = m_tupleData.find(path);
  if ( m_tupleData.find(path) != m_tupleData.end() ) {
    DummyTupleData* p = iTupleData->second;
    if ( p ) delete p;
    m_tupleData.erase( path );
  }
  return true;
}

bool
DummyBackingStore::setTupleData( const std::string& path, DummyTupleData* data)
{
  if ( ! data ) return false;
  std::map< std::string, DummyTupleData* >::iterator iTupleData = m_tupleData.find(path);
  if ( m_tupleData.find(path) != m_tupleData.end() ) {
    iTupleData->second = data;
    return true;
  }
  else return false;
}

IDummyPersistentTuple*
DummyBackingStore::findPersistentTuple( const std::string& path )
{
  std::map< std::string, DummyPersistentTuple >::iterator iTuple = m_tuples.find( path );
  if ( iTuple == m_tuples.end() ) return 0;
  else return &( iTuple->second );
}


bool
DummyBackingStore::bindVariable( AIDA::Dev::ITupleHeader& header, int variableIndex )
{
  const std::string& localPath = header.pathInStore();
  std::map< std::string, DummyPersistentTuple >::iterator iTuple = m_tuples.find( localPath );
  if ( iTuple == m_tuples.end() ) return false;
  return iTuple->second.bindVariable( variableIndex );
}

void*
DummyBackingStore::variableAddress( AIDA::Dev::ITupleHeader& header, int variableIndex )
{
  const std::string& localPath = header.pathInStore();
  std::map< std::string, DummyPersistentTuple >::iterator iTuple = m_tuples.find( localPath );
  if ( iTuple == m_tuples.end() ) return 0;
  return iTuple->second.variableAddress( variableIndex );
}

const void*
DummyBackingStore::variableAddress( const AIDA::Dev::ITupleHeader& header, int variableIndex ) const
{
  const std::string& localPath = header.pathInStore();
  std::map< std::string, DummyPersistentTuple >::const_iterator iTuple = m_tuples.find( localPath );
  if ( iTuple == m_tuples.end() ) return 0;
  return iTuple->second.variableAddress( variableIndex );
}


void
DummyBackingStore::setVariableAddress( const AIDA::Dev::ITupleHeader& header, int variableIndex, void * p ) 
{
}


bool
DummyBackingStore::clearBindings( const AIDA::Dev::ITupleHeader& header )
{
  const std::string& localPath = header.pathInStore();
  std::map< std::string, DummyPersistentTuple >::iterator iTuple = m_tuples.find( localPath );
  if ( iTuple == m_tuples.end() ) return false;
  return iTuple->second.clearBindings();
}

bool
DummyBackingStore::writeTupleRow( AIDA::Dev::ITupleHeader& header )
{
  const std::string& localPath = header.pathInStore();
  std::map< std::string, DummyPersistentTuple >::iterator iTuple = m_tuples.find( localPath );
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
DummyBackingStore::readTupleRow( AIDA::Dev::ITupleHeader& header )
{
  const std::string& localPath = header.pathInStore();
  std::map< std::string, DummyPersistentTuple >::iterator iTuple = m_tuples.find( localPath );
  if ( iTuple == m_tuples.end() ) return false;
  return iTuple->second.readTupleRow( header.currentRowNumber() );
}

bool
DummyBackingStore::resetTuple( AIDA::Dev::ITupleHeader& header )
{
  const std::string& localPath = header.pathInStore();
  std::map< std::string, DummyTupleData* >::iterator iTuple = m_tupleData.find( localPath );
  if ( iTuple == m_tupleData.end() ) return false;
  DummyTupleData& tuple = *( iTuple->second );
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
  for( std::map< int, std::vector< DummyTupleData* > >::iterator i = tuple.tupleVariableData().begin();
       i != tuple.tupleVariableData().end(); ++i ) {
    for ( std::vector< DummyTupleData* >::iterator j = i->second.begin(); j != i->second.end(); ++j ) {
      if ( *j ) delete *j;
    }
    i->second.clear();
  }
  return true;
}
