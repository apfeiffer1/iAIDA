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

#include <typeinfo>
#include <map>
#include <stdexcept>

#include "AIDA_Tuple_native/AIDA_TupleFactory.h"
#include "AIDA_Dev/IDevTree.h"
#include "AIDA_Dev/IDevTupleFactory.h"
#include "AIDA_Dev/IDevFilterFactory.h"
#include "AIDA_Dev/IDevEvaluatorFactory.h"
#include "AIDA_Dev/IDevTuple.h"
#include "AIDA_Dev/IBackingStore.h"
#include "AIDA_Dev/IStore.h"
#include "AIDA_Dev/ITupleHeader.h"
#include "AIDA/IFilter.h"

#include "TupleVariableCopy.h"
#include "TupleVariableDescriptionBuilder.h"


#ifdef IAIDA_AIDA_TUPLE_TUPLE_NO_EXCEPTIONS_TO_USER
#include <iostream>
#define TUPLE_REPORT_ERROR( XXX ) std::cerr << XXX << std::endl
#endif

static const std::string emptyString = "";

static const std::string doubleType = "double";
static const std::string floatType = "float";
static const std::string intType = "int";
static const std::string shortType = "short";
static const std::string longType = "long";
static const std::string charType = "char";
static const std::string boolType = "bool";
static const std::string stringType = "std::string";
static const std::string tupleType = "AIDA::ITuple";


iAIDA::AIDA_Tuple_native::AIDA_TupleFactory::AIDA_TupleFactory( AIDA::Dev::IDevTree& tree,
								 AIDA::Dev::IDevTupleFactory& tupleFactory,
								 AIDA::Dev::IDevFilterFactory& filterFactory,
								 AIDA::Dev::IDevEvaluatorFactory& evaluatorFactory ):
  m_tree( tree ),
  m_tupleFactory( tupleFactory ),
  m_filterFactory( filterFactory ),
  m_evaluatorFactory( evaluatorFactory )
{}


AIDA::ITuple*
iAIDA::AIDA_Tuple_native::AIDA_TupleFactory::create( const std::string& name,
						      const std::string& title,
						      const std::vector<std::string>& columnNames,
						      const std::vector<std::string>& columnTypes,
						      const std::string & options )
{
  try{
    AIDA::Dev::IBackingStore* store = dynamic_cast<AIDA::Dev::IBackingStore*>( m_tree.store(".") );
    if ( ! store ) throw std::runtime_error( "Tuple factory could not access the backing store" );
    AIDA::Dev::IDevTuple* tuple = m_tupleFactory.create( store, title, options );
    if ( tuple ) {
      tuple->setName( name );
      if ( ! m_tree.add( tuple ) ) {
	delete tuple;
	tuple = 0;
      }
      else {
	if ( ! m_tupleFactory.buildTupleHeader(columnNames, columnTypes, m_tree.pathInStore( *tuple ), tuple->header() ) ) { 
	  delete tuple;
	  tuple = 0;
	}
	else if ( ! store->writeTupleHeader( tuple->header() ) ) {
	  delete tuple;
	  tuple = 0;
	}
      }
    }
    return tuple;
  }
  catch( std::bad_cast ) {
    return 0;
  }
#ifdef IAIDA_AIDA_TUPLE_TUPLE_NO_EXCEPTIONS_TO_USER
  catch( std::exception& e ) {
    TUPLE_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


AIDA::ITuple*
iAIDA::AIDA_Tuple_native::AIDA_TupleFactory::create( const std::string& name,
						      const std::string& title,
						      const std::string& columns,
						      const std::string& options )
{
  try{
    AIDA::Dev::IBackingStore* store = dynamic_cast<AIDA::Dev::IBackingStore*>( m_tree.store(".") );
    if ( ! store ) throw std::runtime_error( "Tuple factory could not access the backing store" );
    AIDA::Dev::IDevTuple* tuple = m_tupleFactory.create( store, title, options );
    if ( tuple ) {
      tuple->setName( name );
      if ( ! m_tree.add( tuple ) ) {
	delete tuple;
	tuple = 0;
      }
      else {
	if ( ! m_tupleFactory.buildTupleHeader(columns, m_tree.pathInStore( *tuple ), tuple->header() ) ) { 
	  delete tuple;
	  tuple = 0;
	}
	else if ( ! store->writeTupleHeader( tuple->header() ) ) {
	  delete tuple;
	  tuple = 0;
	}
      }
    }
    return tuple;
  }
  catch( std::bad_cast ) {
    return 0;
  }
#ifdef IAIDA_AIDA_TUPLE_TUPLE_NO_EXCEPTIONS_TO_USER
  catch( std::exception& e ) {
    TUPLE_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


AIDA::ITuple*
iAIDA::AIDA_Tuple_native::AIDA_TupleFactory::createChained( const std::string& name,
							     const std::string& title,
							     const std::vector<AIDA::ITuple*>& tupleSet )
{
#ifdef IAIDA_AIDA_TUPLE_TUPLE_NO_EXCEPTIONS_TO_USER
  try {
#endif
    std::vector<AIDA::Dev::IDevTuple*> tuples;
    for ( unsigned int i = 0; i < tupleSet.size(); ++i ) {
      AIDA::ITuple* p = tupleSet[i];
      if ( !p ) return 0;
      tuples.push_back( dynamic_cast< AIDA::Dev::IDevTuple* >( p ) );
      if ( ! tuples.back() ) return 0;
    }
    AIDA::Dev::IDevTuple* tuple = m_tupleFactory.createChained( title, tuples );
    if ( tuple ) {
      tuple->setName( name );
      if ( ! m_tree.add( tuple ) ) {
	delete tuple;
	tuple = 0;
      }
    }
    return tuple;
#ifdef IAIDA_AIDA_TUPLE_TUPLE_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    TUPLE_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


AIDA::ITuple*
iAIDA::AIDA_Tuple_native::AIDA_TupleFactory::createChained( const std::string& name,
							     const std::string& title,
							     const std::vector<std::string>& tupleSet )
{
#ifdef IAIDA_AIDA_TUPLE_TUPLE_NO_EXCEPTIONS_TO_USER
  try {
#endif
    std::vector<AIDA::Dev::IDevTuple*> tuples;
    for ( unsigned int i = 0; i < tupleSet.size(); ++i ) {
      AIDA::IManagedObject* p = m_tree.find( tupleSet[i] );
      if ( !p ) return 0;
      tuples.push_back( dynamic_cast< AIDA::Dev::IDevTuple* >( p ) );
      if ( ! tuples.back() ) return 0;
    }
    AIDA::Dev::IDevTuple* tuple = m_tupleFactory.createChained( title, tuples );
    if ( tuple ) {
      tuple->setName( name );
      if ( ! m_tree.add( tuple ) ) {
	delete tuple;
	tuple = 0;
      }
    }
    return tuple;
#ifdef IAIDA_AIDA_TUPLE_TUPLE_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    TUPLE_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


AIDA::ITuple*
iAIDA::AIDA_Tuple_native::AIDA_TupleFactory::createFiltered( const std::string& name,
							      AIDA::ITuple& tuple,
							      AIDA::IFilter& filter )
{
  const int ncolumns = tuple.columns();
  std::vector<std::string> names( ncolumns );
  std::vector<std::string> types( ncolumns );
  std::vector< iAIDA::AIDA_Tuple_native::TupleVariableCopyBase* > copyMethods;
  bool typesOK = true;
  for ( int i = 0; i < ncolumns; ++i ) {
    names[i] = tuple.columnName( i );
    std::string type = tuple.columnType( i );
    types[i] = type;

    // need to add sub-tuple description

    if (type == tupleType) {
      std::string stDescr = type; 
      AIDA::ITuple * stp = tuple.getTuple( i); 
      stDescr += " { ";
      iAIDA::AIDA_Tuple_native::TupleVariableDescriptionBuilder::compose(*stp,stDescr); 
      stDescr += " } ";
      types[i] = stDescr;	
    }

    if ( type == doubleType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyDouble );
    else if ( type == floatType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyFloat );
    else if ( type == longType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyLong );
    else if ( type == intType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyInt );
    else if ( type == shortType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyShort );
    else if ( type == charType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyChar );
    else if ( type == boolType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyBoolean );
    else if ( type == stringType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyString );
    else if ( type == tupleType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyTuple );
    else {
      typesOK = false;
      break;
    }
  }
  if ( ! typesOK ) {
    for ( unsigned int i = 0; i < copyMethods.size(); ++i ) delete copyMethods[i];
    return 0;
  }

  AIDA::ITuple* newTuple = this->create( name, tuple.title(), names, types, emptyString );
  if ( ! newTuple ) {
    for ( unsigned int i = 0; i < copyMethods.size(); ++i ) delete copyMethods[i];
    return 0;
  }

  filter.initialize( tuple );
  tuple.start();
  while( tuple.next() ) {
    if ( filter.accept() ) {
      // Copy over the values
      for ( int i = 0; i < ncolumns; ++i ) {
	copyMethods[i]->copyTupleVariable( tuple, i, *newTuple, i );
      }
      // LM move add row after copying values otherwise first row is empty
      newTuple->addRow();
    }
  }

  for ( unsigned int i = 0; i < copyMethods.size(); ++i ) delete copyMethods[i];
  newTuple->start(); // Trick to flush the last row into the store.
  return newTuple;
}


AIDA::ITuple*
iAIDA::AIDA_Tuple_native::AIDA_TupleFactory::createFiltered( const std::string& name,
							      AIDA::ITuple& tuple,
							      AIDA::IFilter& filter,
							      const std::vector<std::string>& columns )
{
  int ncolumns = columns.size();
  std::vector<std::string> names;
  std::vector<std::string> types;
  std::map<int, int> columnFilteredToParent;
  std::vector< iAIDA::AIDA_Tuple_native::TupleVariableCopyBase* > copyMethods;
  bool typesOK = true;
  for ( int i = 0; i < ncolumns; ++i ) {
    const int j = tuple.findColumn( columns[i] );
    if ( j < 0 ) continue;
    names.push_back( tuple.columnName( j ) );
    std::string type = tuple.columnType( j );
    types.push_back( type );
    columnFilteredToParent.insert( std::make_pair( static_cast<int>( names.size() - 1 ), j ) );
    if ( type == doubleType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyDouble );
    else if ( type == floatType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyFloat );
    else if ( type == longType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyLong );
    else if ( type == intType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyInt );
    else if ( type == shortType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyShort );
    else if ( type == charType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyChar );
    else if ( type == boolType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyBoolean );
    else if ( type == stringType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyString );
    else if ( type == tupleType ) copyMethods.push_back( new iAIDA::AIDA_Tuple_native::TupleVarialbeCopyTuple );
    else {
      typesOK = false;
      break;
    }
  }
  if ( ! typesOK ) {
    for ( unsigned int i = 0; i < copyMethods.size(); ++i ) delete copyMethods[i];
    return 0;
  }
  ncolumns = names.size();

  AIDA::ITuple* newTuple = this->create( name, tuple.title(), names, types, emptyString );
  if ( ! newTuple ) {
    for ( unsigned int i = 0; i < copyMethods.size(); ++i ) delete copyMethods[i];
    return 0;
  }
  filter.initialize( tuple );
  tuple.start();
  while( tuple.next() ) {
    if ( filter.accept() ) {
      // Copy over the values
      for ( int i = 0; i < ncolumns; ++i ) {
	copyMethods[i]->copyTupleVariable( tuple, columnFilteredToParent[i], *newTuple, i );
      }
      // LM move add row after copying values otherwise first row is empty
      newTuple->addRow();
    }
  }
  for ( unsigned int i = 0; i < copyMethods.size(); ++i ) delete copyMethods[i];
  newTuple->start(); // Trick to flush the last row into the store.
  return newTuple;
}


AIDA::IFilter*
iAIDA::AIDA_Tuple_native::AIDA_TupleFactory::createFilter( const std::string& expression )
{
  return m_filterFactory.createScripted( expression );
}


AIDA::IFilter*
iAIDA::AIDA_Tuple_native::AIDA_TupleFactory::createFilter( const std::string& expression,
							    int rowsToProcess,
							    int startingRow )
{
  return m_filterFactory.createScripted( expression, rowsToProcess, startingRow );
}


AIDA::IEvaluator*
iAIDA::AIDA_Tuple_native::AIDA_TupleFactory::createEvaluator( const std::string& expression )
{
  return m_evaluatorFactory.createScripted( expression );
}
