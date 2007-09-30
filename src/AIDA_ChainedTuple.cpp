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

#include "AIDA_ChainedTuple.h"
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
#include <stdexcept>
#include <cmath>

static const std::string titleKey = "Title"; // not anymore in aida (from 3.2)
static const std::string nameKey = "Name";


iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::AIDA_ChainedTuple( const std::vector<IDevTuple*>& tuples ):
  m_tuples( tuples ),
  m_header(),
  m_currentTuple( 0 )
{
  if ( tuples.empty() || ! tuples.front() ) {
    throw std::runtime_error( "Not valid set of input tuples to create the chain" );
  }
}


iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::~AIDA_ChainedTuple()
{}


std::string
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::name() const
{
  return m_header.annotation().value( nameKey );
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::isUpToDate() const
{
  return m_tuples.back()->isUpToDate();
}


void
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::setUpToDate( bool isUpToDate )
{
  m_tuples.back()->setUpToDate( isUpToDate );
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::setName( const std::string& newName )
{
  AIDA::IAnnotation& annotation = m_header.annotation();
  annotation.setValue( nameKey, newName );
  return true;
}


const std::string&
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::userLevelClassType() const
{
  return m_tuples.front()->userLevelClassType();
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::connectToStore( AIDA::Dev::IBackingStore* /* store */ )
{
  return true;
}


AIDA::Dev::IBackingStore*
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::store()
{
  return m_tuples.back()->store();
}


const AIDA::Dev::IBackingStore*
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::store() const
{
  return m_tuples.back()->store();
}


AIDA::Dev::ITupleHeader&
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::header()
{
  return m_header;
}


const AIDA::Dev::ITupleHeader&
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::header() const
{
  return m_header;
}


void*
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::variableAddress( const std::string& variableName )
{
  return m_tuples[m_currentTuple]->variableAddress( variableName );
}


const void*
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::variableAddress( const std::string& variableName ) const
{
  return m_tuples[m_currentTuple]->variableAddress( variableName );
}


std::string
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::title() const
{
  return m_header.annotation().value( titleKey );
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::setTitle(const std::string & title)
{
  AIDA::IAnnotation& annotation = m_header.annotation();
  annotation.setValue( titleKey, title );
  return true;
}


AIDA::IAnnotation&
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::annotation()
{
  return m_header.annotation();
}


const AIDA::IAnnotation&
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::annotation() const
{
  return m_header.annotation();
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::fill(int column, double value)
{
  m_currentTuple = m_tuples.size() - 1;
  return m_tuples.back()->fill( column, value );
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::fill(int column, float value)
{
  m_currentTuple = m_tuples.size() - 1;
  return m_tuples.back()->fill( column, value );
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::fill(int column, long value)
{
  m_currentTuple = m_tuples.size() - 1;
  return m_tuples.back()->fill( column, value );
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::fill(int column, int value)
{
  m_currentTuple = m_tuples.size() - 1;
  return m_tuples.back()->fill( column, value );
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::fill(int column, short value)
{
  m_currentTuple = m_tuples.size() - 1;
  return m_tuples.back()->fill( column, value );
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::fill(int column, char value)
{
  m_currentTuple = m_tuples.size() - 1;
  return m_tuples.back()->fill( column, value );
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::fill(int column, bool value)
{
  m_currentTuple = m_tuples.size() - 1;
  return m_tuples.back()->fill( column, value );
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::fill(int column, const std::string& value)
{
  m_currentTuple = m_tuples.size() - 1;
  return m_tuples.back()->fill( column, value );
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::fill(int column, const AIDA::ITupleEntry & value )
{
  m_currentTuple = m_tuples.size() - 1;
  return m_tuples.back()->fill( column, value );
}


void
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::setVariableAddress(int column, void * p )
{
  m_currentTuple = m_tuples.size() - 1;
  return m_tuples.back()->setVariableAddress( column, p );
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::fill(const std::vector<double>& values)
{
  m_currentTuple = m_tuples.size() - 1;
  return m_tuples.back()->fill( values );
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::fill(const std::vector<float>& values)
{
  m_currentTuple = m_tuples.size() - 1;
  return m_tuples.back()->fill( values );
}

 
bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::addRow()
{
  m_currentTuple = m_tuples.size() - 1;
  return m_tuples.back()->addRow();
}


void
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::resetRow()
{
  m_tuples[m_currentTuple]->resetRow();
}


void
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::reset()
{
  m_currentTuple = 0;
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) m_tuples[i]->reset();
}


int
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::rows() const
{
  int nrows = 0;
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) nrows += m_tuples[i]->rows();
  return nrows;
}

void
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::start()
{
  m_currentTuple = 0;
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) m_tuples[i]->start();
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::skip(int rows)
{
  if ( rows < 0 ) return false;
  int currentRowNumber = m_tuples[m_currentTuple]->header().currentRowNumber();
  while ( static_cast<unsigned int>( m_currentTuple ) < m_tuples.size() ) {
    const int rowsInTuple = m_tuples[m_currentTuple]->rows();
    const int rowInTuple = currentRowNumber + rows;
    if ( rowInTuple < rowsInTuple - 1 ) {
      return m_tuples[m_currentTuple]->skip( rows );
    }
    else {
      rows -= rowsInTuple;
      m_currentTuple++;
      currentRowNumber = -1;
    }
  }
  return false;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::next()
{
  while(  m_currentTuple < m_tuples.size() ) {
    if ( m_tuples[m_currentTuple]->next() ) return true;
    ++m_currentTuple;
  }
  return false;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::setRow(int rowIndex)
{
  while(  rowIndex >= -1 && m_currentTuple < m_tuples.size() ) {
    if ( m_tuples[m_currentTuple]->setRow( rowIndex ) ) return true;
    rowIndex -= m_tuples[m_currentTuple]->rows();
    ++m_currentTuple;
  }
  return false;
}


int
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::findColumn(const std::string & name) const
{
  return m_tuples[m_currentTuple]->findColumn( name );
}


double
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::getDouble(int column) const
{
  return m_tuples[m_currentTuple]->getDouble( column );
}


float
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::getFloat(int column) const
{
  return m_tuples[m_currentTuple]->getFloat( column );
}


int
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::getInt(int column) const
{
  return m_tuples[m_currentTuple]->getInt( column );
}


short
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::getShort(int column) const
{
  return m_tuples[m_currentTuple]->getShort( column );
}


long
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::getLong(int column) const
{
  return m_tuples[m_currentTuple]->getLong( column );
}


char
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::getChar(int column) const
{
  return m_tuples[m_currentTuple]->getChar( column );
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::getBoolean(int column) const
{
  return m_tuples[m_currentTuple]->getBoolean( column );
}


std::string
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::getString(int column) const
{
  return m_tuples[m_currentTuple]->getString( column );
}


const AIDA::ITupleEntry*
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::getObject(int column) const
{
  return m_tuples[m_currentTuple]->getObject( column );
}


const AIDA::ITuple*
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::getTuple(int column) const
{
  return m_tuples[m_currentTuple]->getTuple( column );
}


AIDA::ITuple*
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::getTuple(int column)
{
  return m_tuples[m_currentTuple]->getTuple( column );
}


int
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::columns() const
{
  return m_tuples[m_currentTuple]->columns();
}


std::string
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::columnName(int column) const
{
  return m_tuples[m_currentTuple]->columnName( column );
}

std::vector<std::string>
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::columnNames( ) const
{
  return m_tuples[m_currentTuple]->columnNames(  );
}


std::string
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::columnType(int column) const
{
  return m_tuples[m_currentTuple]->columnType( column );
}

std::vector<std::string>
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::columnTypes( ) const
{
  return m_tuples[m_currentTuple]->columnTypes(  );
}


double
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::columnMin(int column) const
{
  double mmin = m_tuples[0]->columnMin( column );
  for ( unsigned int i = 1; i < m_tuples.size(); ++i ) {
    const double tmin = m_tuples[i]->columnMin( column );
    if ( mmin > tmin ) mmin = tmin;
  }
  return mmin;
}


double
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::columnMax(int column) const
{
  double mmax = m_tuples[0]->columnMax( column );
  for ( unsigned int i = 1; i < m_tuples.size(); ++i ) {
    const double tmax = m_tuples[i]->columnMax( column );
    if ( mmax < tmax ) mmax = tmax;
  }
  return mmax;
}


double
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::columnMean(int column) const
{
  double mean = 0;
  int entries = 0;
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    const double tmean = m_tuples[i]->columnMean( column );
    const int tentries = m_tuples[i]->rows();
    entries += tentries;
    mean += tmean * tentries;
  }
  if ( entries > 0 ) return ( mean / entries );
  else return 0;
}


double
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::columnRms(int column) const
{
  double mean = 0;
  double rms = 0;
  int entries = 0;
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    const double tmean = m_tuples[i]->columnMean( column );
    const int tentries = m_tuples[i]->rows();
    const double trms = m_tuples[i]->columnRms( column );
    entries += tentries;
    mean += tmean * tentries;
    rms += tentries * std::abs( tmean*tmean -  trms*trms );
  }
  if ( entries > 0 ) {
    mean /= entries;
    rms /= entries;
    return std::sqrt( std::abs( rms - mean*mean ) );
  }
  else return 0;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::IHistogram1D& histogram,
						       AIDA::IEvaluator& evaluatorX )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( histogram, evaluatorX ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::IHistogram1D& histogram,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IFilter& filter )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( histogram, evaluatorX, filter ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::IHistogram1D& histogram,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& weight )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( histogram, evaluatorX, weight ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::IHistogram1D& histogram,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IFilter& filter,
						       AIDA::IEvaluator& weight )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( histogram, evaluatorX, filter, weight ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::IHistogram2D& histogram,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( histogram, evaluatorX, evaluatorY ) ) return false;
  }
  return true;
}



bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::IHistogram2D& histogram,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY,
						       AIDA::IFilter& filter )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( histogram, evaluatorX, evaluatorY, filter ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::IHistogram2D& histogram,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY,
						       AIDA::IEvaluator& weight )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( histogram, evaluatorX, evaluatorY, weight ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::IHistogram2D& histogram,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY,
						       AIDA::IFilter& filter,
						       AIDA::IEvaluator& weight )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( histogram, evaluatorX, evaluatorY, filter, weight ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::IHistogram3D& histogram,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY,
						       AIDA::IEvaluator& evaluatorZ )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( histogram, evaluatorX, evaluatorY, evaluatorZ ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::IHistogram3D& histogram,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY,
						       AIDA::IEvaluator& evaluatorZ,
						       AIDA::IFilter& filter )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( histogram, evaluatorX, evaluatorY, evaluatorZ, filter ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::IHistogram3D& histogram,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY,
						       AIDA::IEvaluator& evaluatorZ,
						       AIDA::IEvaluator& weight )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( histogram, evaluatorX, evaluatorY, evaluatorZ, weight ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::IHistogram3D& histogram,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY,
						       AIDA::IEvaluator& evaluatorZ,
						       AIDA::IFilter& filter,
						       AIDA::IEvaluator& weight )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( histogram, evaluatorX, evaluatorY, evaluatorZ, filter, weight ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::ICloud1D& cloud,
						       AIDA::IEvaluator& evaluatorX )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( cloud, evaluatorX ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::ICloud1D& cloud,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IFilter& filter )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( cloud, evaluatorX, filter ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::ICloud1D& cloud,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& weight )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( cloud, evaluatorX, weight ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::ICloud1D& cloud,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IFilter& filter,
						       AIDA::IEvaluator& weight )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( cloud, evaluatorX, filter, weight ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::ICloud2D& cloud,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( cloud, evaluatorX, evaluatorY ) ) return false;
  }
  return true;
}



bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::ICloud2D& cloud,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY,
						       AIDA::IFilter& filter )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( cloud, evaluatorX, evaluatorY, filter ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::ICloud2D& cloud,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY,
						       AIDA::IEvaluator& weight )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( cloud, evaluatorX, evaluatorY, weight ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::ICloud2D& cloud,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY,
						       AIDA::IFilter& filter,
						       AIDA::IEvaluator& weight )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( cloud, evaluatorX, evaluatorY, filter, weight ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::ICloud3D& cloud,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY,
						       AIDA::IEvaluator& evaluatorZ )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( cloud, evaluatorX, evaluatorY, evaluatorZ ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::ICloud3D& cloud,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY,
						       AIDA::IEvaluator& evaluatorZ,
						       AIDA::IFilter& filter )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( cloud, evaluatorX, evaluatorY, evaluatorZ, filter ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::ICloud3D& cloud,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY,
						       AIDA::IEvaluator& evaluatorZ,
						       AIDA::IEvaluator& weight )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( cloud, evaluatorX, evaluatorY, evaluatorZ, weight ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::ICloud3D& cloud,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY,
						       AIDA::IEvaluator& evaluatorZ,
						       AIDA::IFilter& filter,
						       AIDA::IEvaluator& weight )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( cloud, evaluatorX, evaluatorY, evaluatorZ, filter, weight ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::IProfile1D& profile,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( profile, evaluatorX, evaluatorY ) ) return false;
  }
  return true;
}



bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::IProfile1D& profile,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY,
						       AIDA::IFilter& filter )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( profile, evaluatorX, evaluatorY, filter ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::IProfile1D& profile,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY,
						       AIDA::IEvaluator& weight )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( profile, evaluatorX, evaluatorY, weight ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::IProfile1D& profile,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY,
						       AIDA::IFilter& filter,
						       AIDA::IEvaluator& weight )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( profile, evaluatorX, evaluatorY, filter, weight ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::IProfile2D& profile,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY,
						       AIDA::IEvaluator& evaluatorZ )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( profile, evaluatorX, evaluatorY, evaluatorZ ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::IProfile2D& profile,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY,
						       AIDA::IEvaluator& evaluatorZ,
						       AIDA::IFilter& filter )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( profile, evaluatorX, evaluatorY, evaluatorZ, filter ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::IProfile2D& profile,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY,
						       AIDA::IEvaluator& evaluatorZ,
						       AIDA::IEvaluator& weight )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( profile, evaluatorX, evaluatorY, evaluatorZ, weight ) ) return false;
  }
  return true;
}


bool
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::project( AIDA::IProfile2D& profile,
						       AIDA::IEvaluator& evaluatorX,
						       AIDA::IEvaluator& evaluatorY,
						       AIDA::IEvaluator& evaluatorZ,
						       AIDA::IFilter& filter,
						       AIDA::IEvaluator& weight )
{
  for ( unsigned int i = 0; i < m_tuples.size(); ++i ) {
    if ( ! m_tuples[i]->project( profile, evaluatorX, evaluatorY, evaluatorZ, filter, weight ) ) return false;
  }
  return true;
}

void * 
iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple::cast(const std::string & className) const 
{
  if (className.find(userLevelClassType()) != std::string::npos ) 
    return const_cast<iAIDA::AIDA_Tuple_native::AIDA_ChainedTuple *> (this);
  else 
    return 0;  
}

