#include "AIDA_Tuple.h"
#include "AIDA_Dev/IBackingStore.h"
#include "AIDA_Dev/ITupleVariableDescription.h"
#include "AIDA_Dev/ITupleVariableStatistics.h"
#include "AIDA/IAnnotation.h"
#include "AIDA/IEvaluator.h"
#include "AIDA/IFilter.h"
#include "AIDA/IHistogram1D.h"
#include "AIDA/IHistogram2D.h"
#include "AIDA/IHistogram3D.h"
#include "AIDA/ICloud1D.h"
#include "AIDA/ICloud2D.h"
#include "AIDA/ICloud3D.h"
#include "AIDA/IProfile1D.h"
#include "AIDA/IProfile2D.h"

#include <iostream>

#include <cmath>

static const std::string titleKey = "Title"; // no more  in aida (from 3.2)
static const std::string nameKey = "Name";
static const std::string emptyString = "";

static const std::string g_userLevelClassType = "ITuple";

static const std::string doubleType = "double";
static const std::string floatType = "float";
static const std::string intType = "int";
static const std::string shortType = "short";
static const std::string longType = "long";
static const std::string charType = "char";
static const std::string boolType = "bool";
static const std::string stringType = "std::string";
static const std::string tupleType = "AIDA::ITuple";

iAIDA::AIDA_Tuple_native::AIDA_Tuple::AIDA_Tuple():
  m_header(),
  m_upToDate(false),
  m_store(0)
{
  AIDA::IAnnotation& annotation = m_header.annotation();
  annotation.addItem( titleKey, emptyString, true );
  annotation.addItem( nameKey, emptyString, true );
}


iAIDA::AIDA_Tuple_native::AIDA_Tuple::~AIDA_Tuple()
{}


std::string
iAIDA::AIDA_Tuple_native::AIDA_Tuple::name() const
{
  return m_header.annotation().value( nameKey );
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::isUpToDate() const
{
  return m_upToDate;
}


void
iAIDA::AIDA_Tuple_native::AIDA_Tuple::setUpToDate( bool isUpToDate )
{
  m_upToDate = isUpToDate;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::setName( const std::string& newName )
{
  AIDA::IAnnotation& annotation = m_header.annotation();
  annotation.setValue( nameKey, newName );
  return true;
}


const std::string&
iAIDA::AIDA_Tuple_native::AIDA_Tuple::userLevelClassType() const
{
  return g_userLevelClassType;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::connectToStore( AIDA::Dev::IBackingStore* store )
{
  if (m_store) return false;
  m_store = store;
  return true;
}


AIDA::Dev::IBackingStore*
iAIDA::AIDA_Tuple_native::AIDA_Tuple::store()
{
  return m_store;
}


const AIDA::Dev::IBackingStore*
iAIDA::AIDA_Tuple_native::AIDA_Tuple::store() const
{
  return m_store;
}


AIDA::Dev::ITupleHeader&
iAIDA::AIDA_Tuple_native::AIDA_Tuple::header()
{
  return m_header;
}


const AIDA::Dev::ITupleHeader&
iAIDA::AIDA_Tuple_native::AIDA_Tuple::header() const
{
  return m_header;
}


void*
iAIDA::AIDA_Tuple_native::AIDA_Tuple::variableAddress( const std::string& variableName )
{
  const int index = m_header.variableIndex( variableName );
  if ( index < 0 ) return 0;
  return m_store->variableAddress( m_header, index );
}


const void*
iAIDA::AIDA_Tuple_native::AIDA_Tuple::variableAddress( const std::string& variableName ) const
{
  const int index = m_header.variableIndex( variableName );
  if ( index < 0 ) return 0;
  const AIDA::Dev::IBackingStore* pstore = m_store;
  return pstore->variableAddress( m_header, index );
}


std::string
iAIDA::AIDA_Tuple_native::AIDA_Tuple::title() const
{
  return m_header.annotation().value( titleKey );
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::setTitle(const std::string & title)
{
  AIDA::IAnnotation& annotation = m_header.annotation();
  annotation.setValue( titleKey, title );
  return true;
}


AIDA::IAnnotation&
iAIDA::AIDA_Tuple_native::AIDA_Tuple::annotation()
{
  return m_header.annotation();
}


const AIDA::IAnnotation&
iAIDA::AIDA_Tuple_native::AIDA_Tuple::annotation() const
{
  return m_header.annotation();
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::fill(int column, double value)
{
  if ( column < 0 || column >= m_header.numberOfVariables() ) return false;
  const std::string& type = m_header.variableDescription( column )->variableType();
  if ( type == doubleType ) return fillChecked( column, value );
  else if ( type == floatType ) return fillChecked( column, static_cast<float>( value ) );
  else if ( type == intType ) return fillChecked( column, static_cast<int>( std::floor( value + 0.5 ) ) );
  else if ( type == longType ) return fillChecked( column, static_cast<long>( std::floor( value + 0.5 ) ) );
  else if ( type == shortType ) return fillChecked( column, static_cast<short>( std::floor( value + 0.5 ) ) );
  else if ( type == charType ) return fillChecked( column, static_cast<char>( std::floor( value + 0.5 ) ) );
  else if ( type == boolType ) return fillChecked( column, ( (value == 0)?false:true) );
  else return false;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::fill(int column, float value)
{
  if ( column < 0 || column >= m_header.numberOfVariables() ) return false;
  const std::string& type = m_header.variableDescription( column )->variableType();
  if ( type == floatType ) return fillChecked( column, value );
  else if ( type == doubleType) return fillChecked( column, static_cast<double>( value ) );
  else if ( type == intType ) return fillChecked( column, static_cast<int>( std::floor( value + 0.5 ) ) );
  else if ( type == longType ) return fillChecked( column, static_cast<long>( std::floor( value + 0.5 ) ) );
  else if ( type == shortType ) return fillChecked( column, static_cast<short>( std::floor( value + 0.5 ) ) );
  else if ( type == charType ) return fillChecked( column, static_cast<char>( std::floor( value + 0.5 ) ) );
  else if ( type == boolType ) return fillChecked( column, ( (value == 0)?false:true) );
  else return false;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::fill(int column, long value)
{
  if ( column < 0 || column >= m_header.numberOfVariables() ) return false;
  const std::string& type = m_header.variableDescription( column )->variableType();
  if ( type == longType ) return fillChecked( column, value );
  else if ( type == doubleType) return fillChecked( column, static_cast<double>( value ) );
  else if ( type == floatType) return fillChecked( column, static_cast<double>( value ) );
  else if ( type == intType ) return fillChecked( column, static_cast<int>( value ) );
  else if ( type == shortType ) return fillChecked( column, static_cast<short>( value ) );
  else if ( type == charType ) return fillChecked( column, static_cast<char>( value ) );
  else if ( type == boolType ) return fillChecked( column, ( (value == 0)?false:true) );
  else return false;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::fill(int column, int value)
{
  if ( column < 0 || column >= m_header.numberOfVariables() ) return false;
  const std::string& type = m_header.variableDescription( column )->variableType();
  if ( type == intType ) return fillChecked( column, value );
  else if ( type == doubleType) return fillChecked( column, static_cast<double>( value ) );
  else if ( type == floatType) return fillChecked( column, static_cast<double>( value ) );
  else if ( type == longType ) return fillChecked( column, static_cast<long>( value ) );
  else if ( type == shortType ) return fillChecked( column, static_cast<short>( value ) );
  else if ( type == charType ) return fillChecked( column, static_cast<char>( value ) );
  else if ( type == boolType ) return fillChecked( column, ( (value == 0)?false:true) );
  else return false;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::fill(int column, short value)
{
  if ( column < 0 || column >= m_header.numberOfVariables() ) return false;
  const std::string& type = m_header.variableDescription( column )->variableType();
  if ( type == shortType ) return fillChecked( column, value );
  else if ( type == doubleType) return fillChecked( column, static_cast<double>( value ) );
  else if ( type == floatType) return fillChecked( column, static_cast<double>( value ) );
  else if ( type == intType ) return fillChecked( column, static_cast<int>( value ) );
  else if ( type == longType ) return fillChecked( column, static_cast<long>( value ) );
  else if ( type == charType ) return fillChecked( column, static_cast<char>( value ) );
  else if ( type == boolType ) return fillChecked( column, ( (value == 0)?false:true) );
  else return false;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::fill(int column, char value)
{
  if ( column < 0 || column >= m_header.numberOfVariables() ) return false;
  const std::string& type = m_header.variableDescription( column )->variableType();
  if ( type == charType ) return fillChecked( column, value );
  else if ( type == doubleType) return fillChecked( column, static_cast<double>( value ) );
  else if ( type == floatType) return fillChecked( column, static_cast<double>( value ) );
  else if ( type == intType ) return fillChecked( column, static_cast<int>( value ) );
  else if ( type == longType ) return fillChecked( column, static_cast<long>( value ) );
  else if ( type == shortType ) return fillChecked( column, static_cast<short>( value ) );
  else if ( type == boolType ) return fillChecked( column, ( (value == 0)?false:true) );
  else return false;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::fill(int column, bool value)
{
  if ( column < 0 || column >= m_header.numberOfVariables() ) return false;
  const std::string& type = m_header.variableDescription( column )->variableType();
  if ( type == boolType ) return fillChecked( column, value );
  else if ( type == doubleType) return fillChecked( column, static_cast<double>( ( (value)?1:0) ) );
  else if ( type == floatType) return fillChecked( column, static_cast<double>( ( (value)?1:0) ) );
  else if ( type == intType ) return fillChecked( column, static_cast<int>( ( (value)?1:0) ) );
  else if ( type == longType ) return fillChecked( column, static_cast<long>( ( (value)?1:0) ) );
  else if ( type == shortType ) return fillChecked( column, static_cast<short>( ( (value)?1:0) ) );
  else if ( type == charType ) return fillChecked( column, static_cast<char>( ( (value)?1:0) ) );
  else return false;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::fill(int column, const std::string& value)
{
  if ( column < 0 || column >= m_header.numberOfVariables() ) return false;
  const std::string& type = m_header.variableDescription( column )->variableType();
  if ( type != stringType ) return false;
  void * p = m_store->variableAddress( m_header, column );
  if ( !p ) return false;
  *( reinterpret_cast<std::string*>( p ) )= value;
  m_header.variableDescription(column)->statistics().addEntry( 0 );
  m_upToDate = false;
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::fill(int /* column */, const AIDA::ITupleEntry &)
{
  return false; // not yet supported
}


// for generic opbects (this is C not C++) 

void 
iAIDA::AIDA_Tuple_native::AIDA_Tuple::setVariableAddress(int column, void * p) { 
   m_store->setVariableAddress( m_header, column, p);
}

bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::fill(const std::vector<double>& values)
{
  bool ok = true;
  for ( int i = 0; i < static_cast<int>( values.size() ); ++i ) {
    if ( ! fill( i, values[i] ) ) {
      ok = false;
      break;
    }
  }
  return ok;
}




bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::fill(const std::vector<float>& values)
{
  bool ok = true;
  for ( int i = 0; i < static_cast<int>( values.size() ); ++i ) {
    if ( ! fill( i, values[i] ) ) {
      ok = false;
      break;
    }
  }
  return ok;
}


 
bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::addRow()
{
  int currentNumberOfRows = m_header.numberOfRows();
  m_header.setCurrentRowNumber( currentNumberOfRows );
  m_header.setNumberOfRows( currentNumberOfRows + 1 );
  if ( ! m_store->writeTupleRow( m_header ) ) {
    m_header.setCurrentRowNumber( currentNumberOfRows - 1);
    m_header.setNumberOfRows( currentNumberOfRows );
    return false;
  }
  //LM: change update the statistics after writing
  const int numberOfColumns = m_header.numberOfVariables();
  for ( int iColumn = 0; iColumn < numberOfColumns; ++iColumn ) {
    const std::string& variableType = m_header.variableDescription( iColumn )->variableType();
    void* p =  m_store->variableAddress( m_header, iColumn );
    if ( variableType == doubleType ) m_header.variableDescription(iColumn)->statistics().addEntry( *(reinterpret_cast<double*>(p) ) );
    else if ( variableType == floatType ) m_header.variableDescription(iColumn)->statistics().addEntry( static_cast<double>( *(reinterpret_cast<float*>(p) ) ) );
    else if ( variableType == longType ) m_header.variableDescription(iColumn)->statistics().addEntry( static_cast<double>( *(reinterpret_cast<long*>(p) ) ) );
    else if ( variableType == intType ) m_header.variableDescription(iColumn)->statistics().addEntry( static_cast<double>( *(reinterpret_cast<int*>(p) ) ) );
    else if ( variableType == shortType ) m_header.variableDescription(iColumn)->statistics().addEntry( static_cast<double>( *(reinterpret_cast<short*>(p) ) ) );
    else if ( variableType == charType ) m_header.variableDescription(iColumn)->statistics().addEntry( static_cast<double>( *(reinterpret_cast<char*>(p) ) ) );
    else if ( variableType == boolType ) m_header.variableDescription(iColumn)->statistics().addEntry( ( ( *(reinterpret_cast<bool*>(p) ) == true ) ? 1 : 0 ) );
    else if ( variableType == stringType ) m_header.variableDescription(iColumn)->statistics().addEntry( 0 );
    else if ( variableType == tupleType ) m_header.variableDescription(iColumn)->statistics().addEntry( 0 );
  }
  m_upToDate = false;
  return true;
}


void
iAIDA::AIDA_Tuple_native::AIDA_Tuple::resetRow()
{
  const int numberOfColumns = m_header.numberOfVariables();
  for ( int iColumn = 0; iColumn < numberOfColumns; ++iColumn ) {
    const std::string& variableType = m_header.variableDescription( iColumn )->variableType();
    void* p =  m_store->variableAddress( m_header, iColumn );
    if ( variableType == doubleType ) *(reinterpret_cast<double*>(p)) = 0;
    else if ( variableType == floatType ) *(reinterpret_cast<float*>(p)) = 0;
    else if ( variableType == longType ) *(reinterpret_cast<long*>(p)) = 0;
    else if ( variableType == intType ) *(reinterpret_cast<int*>(p)) = 0;
    else if ( variableType == shortType ) *(reinterpret_cast<short*>(p)) = 0;
    else if ( variableType == charType ) *(reinterpret_cast<char*>(p)) = 0;
    else if ( variableType == boolType ) *(reinterpret_cast<bool*>(p)) = true;
    else if ( variableType == stringType ) *(reinterpret_cast<std::string*>(p)) = "";
    else if ( variableType == tupleType ) reinterpret_cast<AIDA::Dev::IDevTuple*>(p)->reset();
  }
}


void
iAIDA::AIDA_Tuple_native::AIDA_Tuple::reset()
{
  const int numberOfColumns = m_header.numberOfVariables();
  for ( int iColumn = 0; iColumn < numberOfColumns; ++iColumn ) {
    m_header.variableDescription( iColumn )->resetStatistics();
  }
  m_store->resetTuple( m_header );
  m_header.setNumberOfRows( 0 );
  m_store->clearBindings( m_header );
  m_upToDate = false;
  start();
}


int
iAIDA::AIDA_Tuple_native::AIDA_Tuple::rows() const
{
  return m_header.numberOfRows();
}


void
iAIDA::AIDA_Tuple_native::AIDA_Tuple::start()
{
  m_header.setCurrentRowNumber( -1 );
  // bind all the variables
  const int numberOfColumns = m_header.numberOfVariables();
  for ( int iColumn = 0; iColumn < numberOfColumns; ++iColumn ) {
    m_store->bindVariable( m_header, iColumn );
  }
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::skip(int rows)
{
  if ( rows < 0 ) return false;
  const int nextCurrentRowNumber = rows + m_header.currentRowNumber();
  if ( nextCurrentRowNumber < m_header.numberOfRows() ) {
    m_header.setCurrentRowNumber( nextCurrentRowNumber );
    return m_store->readTupleRow( m_header );
  }
  else return false;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::next()
{
  const int nextCurrentRowNumber = m_header.currentRowNumber() + 1;
  if ( nextCurrentRowNumber < m_header.numberOfRows() ) {
    m_header.setCurrentRowNumber( nextCurrentRowNumber );
    return m_store->readTupleRow( m_header );
  }
  else return false;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::setRow(int rowIndex)
{
  if ( rowIndex < -1 || rowIndex >= m_header.numberOfRows() ) return false;
  m_header.setCurrentRowNumber( rowIndex );
  return m_store->readTupleRow( m_header );
}


int
iAIDA::AIDA_Tuple_native::AIDA_Tuple::findColumn(const std::string & name) const
{
  return m_header.variableIndex( name );
}


double
iAIDA::AIDA_Tuple_native::AIDA_Tuple::getDouble(int column) const
{
  return *( getVariable<double>( column ) );
}


float
iAIDA::AIDA_Tuple_native::AIDA_Tuple::getFloat(int column) const
{
  return *( getVariable<float>( column ) );
}


int
iAIDA::AIDA_Tuple_native::AIDA_Tuple::getInt(int column) const
{
  return *( getVariable<int>( column ) );
}


short
iAIDA::AIDA_Tuple_native::AIDA_Tuple::getShort(int column) const
{
  return *( getVariable<short>( column ) );
}


long
iAIDA::AIDA_Tuple_native::AIDA_Tuple::getLong(int column) const
{
  return *( getVariable<long>( column ) );
}


char
iAIDA::AIDA_Tuple_native::AIDA_Tuple::getChar(int column) const
{
  return *( getVariable<char>( column ) );
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::getBoolean(int column) const
{
  return *( getVariable<bool>( column ) );
}


std::string
iAIDA::AIDA_Tuple_native::AIDA_Tuple::getString(int column) const
{
  return *( getVariable<std::string>( column ) );
}


const AIDA::ITupleEntry*
iAIDA::AIDA_Tuple_native::AIDA_Tuple::getObject(int /*column*/ ) const
{
  return 0;
}


const AIDA::ITuple*
iAIDA::AIDA_Tuple_native::AIDA_Tuple::getTuple(int column) const
{
  return static_cast<const AIDA::ITuple*>( getVariable<AIDA::Dev::IDevTuple>( column ) );
}


AIDA::ITuple*
iAIDA::AIDA_Tuple_native::AIDA_Tuple::getTuple(int column)
{
  return static_cast<AIDA::ITuple*>( getVariable<AIDA::Dev::IDevTuple>( column ) );
}


int
iAIDA::AIDA_Tuple_native::AIDA_Tuple::columns() const
{
  return m_header.numberOfVariables();
}


std::string
iAIDA::AIDA_Tuple_native::AIDA_Tuple::columnName(int column) const
{
  const AIDA::Dev::ITupleVariableDescription* description = m_header.variableDescription(column);
  if ( ! description ) return emptyString;
  return description->variableName();
}

std::vector<std::string>
iAIDA::AIDA_Tuple_native::AIDA_Tuple::columnNames( ) const
{
  std::vector<std::string> names(columns());
  for (unsigned int i = 0; i < names.size() ; ++i) 
    names[i] = columnName(i); 
  return names;
}


std::string
iAIDA::AIDA_Tuple_native::AIDA_Tuple::columnType(int column) const
{
  const AIDA::Dev::ITupleVariableDescription* description = m_header.variableDescription(column);
  if ( ! description ) return emptyString;
  return description->variableType();
}

std::vector<std::string>
iAIDA::AIDA_Tuple_native::AIDA_Tuple::columnTypes( ) const
{
  std::vector<std::string> names( columns() ); 
  for (unsigned int i = 0; i < names.size() ; ++i) 
    names[i] = columnType(i); 
  return names;
}


double
iAIDA::AIDA_Tuple_native::AIDA_Tuple::columnMin(int column) const
{
  const AIDA::Dev::ITupleVariableDescription* description = m_header.variableDescription(column);
  if ( ! description ) return 0;
  return description->statistics().min();
}


double
iAIDA::AIDA_Tuple_native::AIDA_Tuple::columnMax(int column) const
{
  const AIDA::Dev::ITupleVariableDescription* description = m_header.variableDescription(column);
  if ( ! description ) return 0;
  return description->statistics().max();
}


double
iAIDA::AIDA_Tuple_native::AIDA_Tuple::columnMean(int column) const
{
  const AIDA::Dev::ITupleVariableDescription* description = m_header.variableDescription(column);
  if ( ! description ) return 0;
  return description->statistics().mean();
}


double
iAIDA::AIDA_Tuple_native::AIDA_Tuple::columnRms(int column) const
{
  const AIDA::Dev::ITupleVariableDescription* description = m_header.variableDescription(column);
  if ( ! description ) return 0;
  return description->statistics().rms();
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::IHistogram1D& histogram,
						AIDA::IEvaluator& evaluatorX )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    histogram.fill( evaluatorX.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::IHistogram1D& histogram,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IFilter& filter )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! filter.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    if ( filter.accept() ) histogram.fill( evaluatorX.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::IHistogram1D& histogram,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& weight )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! weight.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    histogram.fill( evaluatorX.evaluateDouble(), weight.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::IHistogram1D& histogram,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IFilter& filter,
						AIDA::IEvaluator& weight )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! filter.initialize( *this ) ) return false;
  if ( ! weight.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    if ( filter.accept() ) histogram.fill( evaluatorX.evaluateDouble(), weight.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::IHistogram2D& histogram,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    histogram.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::IHistogram2D& histogram,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY,
						AIDA::IFilter& filter )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  filter.initialize( *this );
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    if ( filter.accept() ) histogram.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::IHistogram2D& histogram,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY,
						AIDA::IEvaluator& weight )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  if ( ! weight.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    histogram.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble(), weight.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::IHistogram2D& histogram,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY,
						AIDA::IFilter& filter,
						AIDA::IEvaluator& weight )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  if ( ! filter.initialize( *this ) ) return false;
  if ( ! weight.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    if ( filter.accept() ) histogram.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble(), weight.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::IHistogram3D& histogram,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY,
						AIDA::IEvaluator& evaluatorZ )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  if ( ! evaluatorZ.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    histogram.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble(), evaluatorZ.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::IHistogram3D& histogram,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY,
						AIDA::IEvaluator& evaluatorZ,
						AIDA::IFilter& filter )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  if ( ! evaluatorZ.initialize( *this ) ) return false;
  if ( ! filter.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    if ( filter.accept() ) histogram.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble(), evaluatorZ.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::IHistogram3D& histogram,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY,
						AIDA::IEvaluator& evaluatorZ,
						AIDA::IEvaluator& weight )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  if ( ! evaluatorZ.initialize( *this ) ) return false;
  if ( ! weight.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    histogram.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble(), evaluatorZ.evaluateDouble(), weight.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::IHistogram3D& histogram,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY,
						AIDA::IEvaluator& evaluatorZ,
						AIDA::IFilter& filter,
						AIDA::IEvaluator& weight )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  if ( ! evaluatorZ.initialize( *this ) ) return false;
  if ( ! weight.initialize( *this ) ) return false;
  if ( ! filter.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    if ( filter.accept() ) histogram.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble(), evaluatorZ.evaluateDouble(), weight.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::ICloud1D& cloud,
						AIDA::IEvaluator& evaluatorX )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    cloud.fill( evaluatorX.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::ICloud1D& cloud,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IFilter& filter )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! filter.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    if ( filter.accept() ) cloud.fill( evaluatorX.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::ICloud1D& cloud,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& weight )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! weight.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    cloud.fill( evaluatorX.evaluateDouble(), weight.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::ICloud1D& cloud,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IFilter& filter,
						AIDA::IEvaluator& weight )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! filter.initialize( *this ) ) return false;
  if ( ! weight.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    if ( filter.accept() ) cloud.fill( evaluatorX.evaluateDouble(), weight.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::ICloud2D& cloud,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    cloud.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::ICloud2D& cloud,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY,
						AIDA::IFilter& filter )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  if ( ! filter.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    if ( filter.accept() ) cloud.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::ICloud2D& cloud,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY,
						AIDA::IEvaluator& weight )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  if ( ! weight.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    cloud.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble(), weight.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::ICloud2D& cloud,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY,
						AIDA::IFilter& filter,
						AIDA::IEvaluator& weight )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  if ( ! filter.initialize( *this ) ) return false;
  if ( ! weight.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    if ( filter.accept() ) cloud.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble(), weight.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::ICloud3D& cloud,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY,
						AIDA::IEvaluator& evaluatorZ )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  if ( ! evaluatorZ.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    cloud.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble(), evaluatorZ.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::ICloud3D& cloud,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY,
						AIDA::IEvaluator& evaluatorZ,
						AIDA::IFilter& filter )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  if ( ! evaluatorZ.initialize( *this ) ) return false;
  if ( ! filter.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    if ( filter.accept() ) cloud.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble(), evaluatorZ.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::ICloud3D& cloud,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY,
						AIDA::IEvaluator& evaluatorZ,
						AIDA::IEvaluator& weight )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  if ( ! evaluatorZ.initialize( *this ) ) return false;
  if ( ! weight.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    cloud.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble(), evaluatorZ.evaluateDouble(), weight.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::ICloud3D& cloud,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY,
						AIDA::IEvaluator& evaluatorZ,
						AIDA::IFilter& filter,
						AIDA::IEvaluator& weight )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  if ( ! evaluatorZ.initialize( *this ) ) return false;
  if ( ! weight.initialize( *this ) ) return false;
  if ( ! filter.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    if ( filter.accept() ) cloud.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble(), evaluatorZ.evaluateDouble(), weight.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::IProfile1D& profile,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    profile.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::IProfile1D& profile,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY,
						AIDA::IFilter& filter )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  if ( ! filter.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    if ( filter.accept() ) profile.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::IProfile1D& profile,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY,
						AIDA::IEvaluator& weight )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  if ( ! weight.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    profile.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble(), weight.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::IProfile1D& profile,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY,
						AIDA::IFilter& filter,
						AIDA::IEvaluator& weight )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  if ( ! filter.initialize( *this ) ) return false;
  if ( ! weight.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    if ( filter.accept() ) profile.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble(), weight.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::IProfile2D& profile,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY,
						AIDA::IEvaluator& evaluatorZ )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  if ( ! evaluatorZ.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    profile.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble(), evaluatorZ.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::IProfile2D& profile,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY,
						AIDA::IEvaluator& evaluatorZ,
						AIDA::IFilter& filter )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  if ( ! evaluatorZ.initialize( *this ) ) return false;
  if ( ! filter.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    if ( filter.accept() ) profile.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble(), evaluatorZ.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::IProfile2D& profile,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY,
						AIDA::IEvaluator& evaluatorZ,
						AIDA::IEvaluator& weight )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  if ( ! evaluatorZ.initialize( *this ) ) return false;
  if ( ! weight.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    profile.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble(), evaluatorZ.evaluateDouble(), weight.evaluateDouble() );
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_Tuple::project( AIDA::IProfile2D& profile,
						AIDA::IEvaluator& evaluatorX,
						AIDA::IEvaluator& evaluatorY,
						AIDA::IEvaluator& evaluatorZ,
						AIDA::IFilter& filter,
						AIDA::IEvaluator& weight )
{
  m_store->clearBindings( m_header );
  if ( ! evaluatorX.initialize( *this ) ) return false;
  if ( ! evaluatorY.initialize( *this ) ) return false;
  if ( ! evaluatorZ.initialize( *this ) ) return false;
  if ( ! weight.initialize( *this ) ) return false;
  if ( ! filter.initialize( *this ) ) return false;
  const int numberOfRows = m_header.numberOfRows();
  for ( int iRow = 0; iRow < numberOfRows; ++iRow ) {
    m_header.setCurrentRowNumber( iRow );
    m_store->readTupleRow( m_header );
    if ( filter.accept() ) profile.fill( evaluatorX.evaluateDouble(), evaluatorY.evaluateDouble(), evaluatorZ.evaluateDouble(), weight.evaluateDouble() );
  }
  return true;
}

void * 
iAIDA::AIDA_Tuple_native::AIDA_Tuple::cast(const std::string & className) const 
{
  if (className.find(userLevelClassType()) != std::string::npos ) 
    return const_cast<iAIDA::AIDA_Tuple_native::AIDA_Tuple *> (this);
  else 
    return 0;  
}

