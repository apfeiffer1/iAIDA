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

#include "AIDA_MemoryStore/MemoryPersistentTuple.h"
#include "AIDA_MemoryStore/MemoryTupleData.h"
#include "AIDA_MemoryStore/IMemoryBackingStore.h"
#include "AIDA_Dev/ITupleVariableDescription.h"
#include "AIDA_Dev/IDevTuple.h"
#include "AIDA_Dev/ITupleHeader.h"
#include "AIDA_Dev/IDevTupleFactory.h"

static const std::string subTuplePrefix = "st";

iAIDA::AIDA_MemoryStore::MemoryPersistentTuple::MemoryPersistentTuple( IMemoryBackingStore* store,
									AIDA::Dev::IDevTupleFactory& factory,
									iAIDA::AIDA_MemoryStore::MemoryTupleData* data,
									const std::string& pathInStore ):
  m_store( store ),
  m_factory( factory ),
  m_tupleData( data ),
  m_pathInStore( pathInStore ),
  m_currentRow( -1 ),
  m_cache( data->variables().size(), 0 )
{}

iAIDA::AIDA_MemoryStore::MemoryPersistentTuple::~MemoryPersistentTuple()
{
  for ( unsigned int i = 0; i < m_cache.size(); ++i ) {
    void* p = m_cache[i];
    if ( !p ) continue;
    const std::string& variableType = m_tupleData->variables()[i]->variableType();
    if ( variableType == "double" ) delete reinterpret_cast< double * >( p );
    else if ( variableType == "float" ) delete reinterpret_cast< float * >( p );
    else if ( variableType == "long" ) delete reinterpret_cast< long * >( p );
    else if ( variableType == "int" ) delete reinterpret_cast< int * >( p );
    else if ( variableType == "short" ) delete reinterpret_cast< short * >( p );
    else if ( variableType == "char" ) delete reinterpret_cast< char * >( p );
    else if ( variableType == "bool" ) delete reinterpret_cast< bool * >( p );
    else if ( variableType == "std::string" ) delete reinterpret_cast< std::string * >( p );
  }
}

bool
iAIDA::AIDA_MemoryStore::MemoryPersistentTuple::setTupleData( iAIDA::AIDA_MemoryStore::MemoryTupleData* data )
{
  if ( data == 0 ) return false;
  else {
    m_tupleData = data;
    return true;
  }
}

iAIDA::AIDA_MemoryStore::MemoryTupleData*
iAIDA::AIDA_MemoryStore::MemoryPersistentTuple::tupleData() const
{
  return m_tupleData;
}

bool
iAIDA::AIDA_MemoryStore::MemoryPersistentTuple::bindVariable( int variableIndex )
{
  void* & p = m_cache[variableIndex];
  if ( !p ) {
    AIDA::Dev::ITupleVariableDescription& description = *( m_tupleData->variables()[variableIndex] );
    const std::string& variableType = description.variableType();
    if ( variableType == "double" ) p = new double;
    else if ( variableType == "float" ) p = new float;
    else if ( variableType == "long" ) p = new long;
    else if ( variableType == "int" ) p = new int;
    else if ( variableType == "short" ) p = new short;
    else if ( variableType == "char" ) p = new char;
    else if ( variableType == "bool" ) p = new bool;
    else if ( variableType == "std::string" ) p = new std::string;
    else if ( variableType == "AIDA::ITuple" ) {
      AIDA::Dev::IDevTuple* tuple = m_factory.create( m_store, m_tupleData->title() );
      // define the path in store
      std::string path = subTuplePrefix;
      for ( int i = 0; i < variableIndex; ++i ) path += "_";
      path += m_pathInStore;
      tuple->header().setPathInStore( path );
      // Propagate the description of the variables.
      for ( int i = 0; i < description.numberOfVariables(); ++i ) {
	if ( ! tuple->header().setVariableDescription( m_factory.createDescription( *( description.variableDescription(i) ) ),
						       true ) ) return false;
      }
      // write the tuple header
      if ( ! m_store->writeTupleHeader( tuple->header() ) ) return false;
      p = tuple;
    }
  }
  return true;
}

bool
iAIDA::AIDA_MemoryStore::MemoryPersistentTuple::clearBindings()
{
  for ( unsigned int i = 0; i < m_cache.size(); ++i ) {
    void* & p = m_cache[i];
    if ( !p ) continue;
    const std::string& variableType = m_tupleData->variables()[i]->variableType();
    if ( variableType == "double" ) delete reinterpret_cast< double * >( p );
    else if ( variableType == "float" ) delete reinterpret_cast< float * >( p );
    else if ( variableType == "long" ) delete reinterpret_cast< long * >( p );
    else if ( variableType == "int" ) delete reinterpret_cast< int * >( p );
    else if ( variableType == "short" ) delete reinterpret_cast< short * >( p );
    else if ( variableType == "char" ) delete reinterpret_cast< char * >( p );
    else if ( variableType == "bool" ) delete reinterpret_cast< bool * >( p );
    else if ( variableType == "std::string" ) delete reinterpret_cast< std::string * >( p );
    else if ( variableType == "AIDA::ITuple" ) {
      AIDA::Dev::IDevTuple* tuple = reinterpret_cast< AIDA::Dev::IDevTuple * >( p );
      m_store->removePersistentTuple( tuple->header().pathInStore() );
      delete tuple;
    }
    p = 0;
  }
  return true;
}

bool
iAIDA::AIDA_MemoryStore::MemoryPersistentTuple::writeTupleRow( int rowNumber )
{
  if ( rowNumber < 0 ) return false;
  for ( unsigned int i = 0; i < m_cache.size(); ++i ) {
    void* p = m_cache[i];
    const std::string& variableType = m_tupleData->variables()[i]->variableType();
    if ( variableType == "double" ) {
      std::vector< double >& vec = m_tupleData->doubleVariableData().find(i)->second;
      while ( static_cast<unsigned int>( rowNumber ) >= vec.size() ) vec.push_back( 0 );
      if ( p ) vec[rowNumber] = *( reinterpret_cast< double * >( p ) );
    }
    else if ( variableType == "float" ) {
      std::vector< float >& vec = m_tupleData->floatVariableData().find(i)->second;
      while ( static_cast<unsigned int>( rowNumber ) >= vec.size() ) vec.push_back( 0 );
      if ( p ) vec[rowNumber] = *( reinterpret_cast< float * >( p ) );
    }
    else if ( variableType == "long" ) {
      std::vector< long >& vec = m_tupleData->longVariableData().find(i)->second;
      while ( static_cast<unsigned int>( rowNumber ) >= vec.size() ) vec.push_back( 0 );
      if ( p ) vec[rowNumber] = *( reinterpret_cast< long * >( p ) );
    }
    else if ( variableType == "int" ) {
      std::vector< int >& vec = m_tupleData->intVariableData().find(i)->second;
      while ( static_cast<unsigned int>( rowNumber ) >= vec.size() ) vec.push_back( 0 );
      if ( p ) vec[rowNumber] = *( reinterpret_cast< int * >( p ) );
    }
    else if ( variableType == "short" ) {
      std::vector< short >& vec = m_tupleData->shortVariableData().find(i)->second;
      while ( static_cast<unsigned int>( rowNumber ) >= vec.size() ) vec.push_back( 0 );
      if ( p ) vec[rowNumber] = *( reinterpret_cast< short * >( p ) );
    }
    else if ( variableType == "char" ) {
      std::vector< char >& vec = m_tupleData->charVariableData().find(i)->second;
      while ( static_cast<unsigned int>( rowNumber ) >= vec.size() ) vec.push_back( 0 );
      if ( p ) vec[rowNumber] = *( reinterpret_cast< char * >( p ) );
    }
    else if ( variableType == "bool" ) {
      std::vector< bool >& vec = m_tupleData->boolVariableData().find(i)->second;
      while ( static_cast<unsigned int>( rowNumber ) >= vec.size() ) vec.push_back( false );
      if ( p ) vec[rowNumber] = *( reinterpret_cast< bool * >( p ) );
    }
    else if ( variableType == "std::string" ) {
      std::vector< std::string >& vec = m_tupleData->stringVariableData().find(i)->second;
      while ( static_cast<unsigned int>( rowNumber ) >= vec.size() ) vec.push_back( "" );
      if ( p ) vec[rowNumber] = *( reinterpret_cast< std::string * >( p ) );
    }
    else if ( variableType == "AIDA::ITuple" ) {
      std::vector< iAIDA::AIDA_MemoryStore::MemoryTupleData* >& vec = m_tupleData->tupleVariableData().find(i)->second;
      while ( static_cast<unsigned int>( rowNumber ) >= vec.size() ) vec.push_back( 0 );
      if ( p ) {
	AIDA::Dev::IDevTuple* tuple = reinterpret_cast< AIDA::Dev::IDevTuple * >( p );
	const std::string& pathInStore = tuple->header().pathInStore();
        iAIDA::AIDA_MemoryStore::IMemoryPersistentTuple* ptuple = m_store->findPersistentTuple( pathInStore );
	if ( ! ptuple ) return false;
        vec[rowNumber] = ptuple->tupleData();
        iAIDA::AIDA_MemoryStore::MemoryTupleData* data = new MemoryTupleData;
	// copy over the description in the tuple data.
        for ( int i = 0; i < tuple->header().numberOfVariables(); ++i ) {
	  AIDA::Dev::ITupleVariableDescription* description = tuple->header().variableDescription( i );
	  data->variables().push_back( m_factory.createDescription( *description ) );
	  description->resetStatistics();
	  const std::string& type = description->variableType();
	  if ( type == "double" ) data->doubleVariableData().insert( std::make_pair( i, std::vector< double >() ) );
	  else if ( type == "float" ) data->floatVariableData().insert( std::make_pair( i, std::vector< float >() ) );
	  else if ( type == "long" ) data->longVariableData().insert( std::make_pair( i, std::vector< long >() ) );
	  else if ( type == "int" ) data->intVariableData().insert( std::make_pair( i, std::vector< int >() ) );
	  else if ( type == "short" ) data->shortVariableData().insert( std::make_pair( i, std::vector< short >() ) );
	  else if ( type == "char" ) data->charVariableData().insert( std::make_pair( i, std::vector< char >() ) );
	  else if ( type == "bool" ) data->boolVariableData().insert( std::make_pair( i, std::vector< bool >() ) );
	  else if ( type == "std::string" ) data->stringVariableData().insert( std::make_pair( i, std::vector< std::string >() ) );
	  else if ( type == "AIDA::ITuple" ) data->tupleVariableData().insert( std::make_pair( i, std::vector< iAIDA::AIDA_MemoryStore::MemoryTupleData * >() ) );
	}
	ptuple->setTupleData( data );
	m_store->setTupleData( pathInStore, data );
	// Reset the tuple header
	tuple->header().setNumberOfRows( 0 );
	tuple->header().setCurrentRowNumber( -1 );
      }
    }
  }
  return true;
}

bool
iAIDA::AIDA_MemoryStore::MemoryPersistentTuple::readTupleRow( int rowNumber )
{
  if ( rowNumber < 0 ) return false;
  for ( unsigned int i = 0; i < m_cache.size(); ++i ) {
    void* & p = m_cache[i];
    if ( !p ) continue;
    const std::string& variableType = m_tupleData->variables()[i]->variableType();
    if ( variableType == "double" ) {
      const std::vector< double >& vec = m_tupleData->doubleVariableData().find(i)->second;
      if ( static_cast<unsigned int>( rowNumber ) < vec.size() ) *( reinterpret_cast< double * >( p ) ) = vec[rowNumber];
      else return false;
    }
    else if ( variableType == "float" ) {
      const std::vector< float >& vec = m_tupleData->floatVariableData().find(i)->second;
      if ( static_cast<unsigned int>( rowNumber ) < vec.size() ) *( reinterpret_cast< float * >( p ) ) = vec[rowNumber];
      else return false;
    }
    else if ( variableType == "long" ) {
      const std::vector< long >& vec = m_tupleData->longVariableData().find(i)->second;
      if ( static_cast<unsigned int>( rowNumber ) < vec.size() ) *( reinterpret_cast< long * >( p ) ) = vec[rowNumber];
      else return false;
    }
    else if ( variableType == "int" ) {
      const std::vector< int >& vec = m_tupleData->intVariableData().find(i)->second;
      if ( static_cast<unsigned int>( rowNumber ) < vec.size() ) *( reinterpret_cast< int * >( p ) ) = vec[rowNumber];
      else return false;
    }
    else if ( variableType == "short" ) {
      const std::vector< short >& vec = m_tupleData->shortVariableData().find(i)->second;
      if ( static_cast<unsigned int>( rowNumber ) < vec.size() ) *( reinterpret_cast< short * >( p ) ) = vec[rowNumber];
      else return false;
    }
    else if ( variableType == "char" ) {
      const std::vector< char >& vec = m_tupleData->charVariableData().find(i)->second;
      if ( static_cast<unsigned int>( rowNumber ) < vec.size() ) *( reinterpret_cast< char * >( p ) ) = vec[rowNumber];
      else return false;
    }
    else if ( variableType == "bool" ) {
      const std::vector< bool >& vec = m_tupleData->boolVariableData().find(i)->second;
      if ( static_cast<unsigned int>( rowNumber ) < vec.size() ) *( reinterpret_cast< bool * >( p ) ) = vec[rowNumber];
      else return false;
    }
    else if ( variableType == "std::string" ) {
      const std::vector< std::string >& vec = m_tupleData->stringVariableData().find(i)->second;
      if ( static_cast<unsigned int>( rowNumber ) < vec.size() ) *( reinterpret_cast< std::string * >( p ) ) = vec[rowNumber];
      else return false;
   }
    else if ( variableType == "AIDA::ITuple" ) {
      const std::vector< iAIDA::AIDA_MemoryStore::MemoryTupleData* >& vec = m_tupleData->tupleVariableData().find(i)->second;
      if ( static_cast<unsigned int>( rowNumber ) < vec.size() ) {
	iAIDA::AIDA_MemoryStore::MemoryTupleData* data = vec[rowNumber];
	AIDA::Dev::ITupleHeader& tupleHeader = ( reinterpret_cast< AIDA::Dev::IDevTuple * >( p ) )->header();
	m_store->findPersistentTuple( tupleHeader.pathInStore() )->setTupleData( data );
	for ( unsigned int i = 0; i < data->variables().size(); ++i ) {
	  tupleHeader.setVariableDescription( data->variables()[i], false );
	}
	unsigned int nRows = 0;
	if ( data->doubleVariableData().size() > 0 ) nRows = data->doubleVariableData().begin()->second.size();
	else if ( data->floatVariableData().size() > 0 ) nRows = data->floatVariableData().begin()->second.size();
	else if ( data->longVariableData().size() > 0 ) nRows = data->longVariableData().begin()->second.size();
	else if ( data->intVariableData().size() > 0 ) nRows = data->intVariableData().begin()->second.size();
	else if ( data->shortVariableData().size() > 0 ) nRows = data->shortVariableData().begin()->second.size();
	else if ( data->charVariableData().size() > 0 ) nRows = data->charVariableData().begin()->second.size();
	else if ( data->boolVariableData().size() > 0 ) nRows = data->boolVariableData().begin()->second.size();
	else if ( data->stringVariableData().size() > 0 ) nRows = data->stringVariableData().begin()->second.size();
	else if ( data->tupleVariableData().size() > 0 ) nRows = data->tupleVariableData().begin()->second.size();
	tupleHeader.setNumberOfRows( static_cast< int >( nRows ) );
	tupleHeader.setCurrentRowNumber( -1 );
      }
      else return false;
    }
  }
  return true;
}

void*
iAIDA::AIDA_MemoryStore::MemoryPersistentTuple::variableAddress( int variableIndex )
{
  bindVariable( variableIndex );
  return m_cache[variableIndex];
}

const void*
iAIDA::AIDA_MemoryStore::MemoryPersistentTuple::variableAddress( int variableIndex ) const
{
  const_cast<iAIDA::AIDA_MemoryStore::MemoryPersistentTuple*>(this)->bindVariable( variableIndex );
  return m_cache[variableIndex];
}
