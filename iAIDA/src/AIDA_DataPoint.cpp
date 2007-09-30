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

#include "AIDA_DataPoint.h"
#include "AIDA_Measurement.h"

iAIDA::AIDA_DataPointSet_native::AIDA_DataPoint::AIDA_DataPoint( int dimension ):
  m_dimension( dimension ),
  m_measurements( 0 )
{
  if ( m_dimension < 0 ) m_dimension = 0;
  if ( m_dimension > 0 ) {
    m_measurements = new AIDA::IMeasurement*[ m_dimension ];
    for ( int iMeasurement = 0; iMeasurement < m_dimension; ++iMeasurement ) {
      m_measurements[ iMeasurement ] = new iAIDA::AIDA_DataPointSet_native::AIDA_Measurement;
    }
  }
}


iAIDA::AIDA_DataPointSet_native::AIDA_DataPoint::AIDA_DataPoint( const iAIDA::AIDA_DataPointSet_native::AIDA_DataPoint& dp ):
  m_dimension( dp.dimension() ),
  m_measurements( 0 )
{
  if ( m_dimension > 0 ) {
    m_measurements = new AIDA::IMeasurement*[ m_dimension ];
    for ( int iMeasurement = 0; iMeasurement < m_dimension; ++iMeasurement ) {
      m_measurements[ iMeasurement ] = new iAIDA::AIDA_DataPointSet_native::AIDA_Measurement;
    }
  }

  for ( int j = 0; j < m_dimension; ++j ) {
    this->coordinate( j )->setValue( dp.coordinate( j )->value() );
    this->coordinate( j )->setErrorPlus( dp.coordinate( j )->errorPlus() );
    this->coordinate( j )->setErrorMinus( dp.coordinate( j )->errorMinus() );
  }
}


iAIDA::AIDA_DataPointSet_native::AIDA_DataPoint::AIDA_DataPoint( const AIDA::IDataPoint& dp ):
  m_dimension( dp.dimension() ),
  m_measurements( 0 )
{
  if ( m_dimension > 0 ) {
    m_measurements = new AIDA::IMeasurement*[ m_dimension ];
    for ( int iMeasurement = 0; iMeasurement < m_dimension; ++iMeasurement ) {
      m_measurements[ iMeasurement ] = new iAIDA::AIDA_DataPointSet_native::AIDA_Measurement;
    }
  }

  for ( int j = 0; j < m_dimension; ++j ) {
    this->coordinate( j )->setValue( dp.coordinate( j )->value() );
    this->coordinate( j )->setErrorPlus( dp.coordinate( j )->errorPlus() );
    this->coordinate( j )->setErrorMinus( dp.coordinate( j )->errorMinus() );
  }
}


iAIDA::AIDA_DataPointSet_native::AIDA_DataPoint&
iAIDA::AIDA_DataPointSet_native::AIDA_DataPoint::operator=( const AIDA_DataPoint& dp )
{
  if ( dp.dimension() != m_dimension ) return *this;
  for ( int j = 0; j < m_dimension; ++j ) {
    this->coordinate( j )->setValue( dp.coordinate( j )->value() );
    this->coordinate( j )->setErrorPlus( dp.coordinate( j )->errorPlus() );
    this->coordinate( j )->setErrorMinus( dp.coordinate( j )->errorMinus() );
  }
  return *this;
}


AIDA::IDataPoint&
iAIDA::AIDA_DataPointSet_native::AIDA_DataPoint::operator=( const AIDA::IDataPoint& dp )
{
  if ( dp.dimension() != m_dimension ) return *this;
  for ( int j = 0; j < m_dimension; ++j ) {
    this->coordinate( j )->setValue( dp.coordinate( j )->value() );
    this->coordinate( j )->setErrorPlus( dp.coordinate( j )->errorPlus() );
    this->coordinate( j )->setErrorMinus( dp.coordinate( j )->errorMinus() );
  }
  return *this;
}


iAIDA::AIDA_DataPointSet_native::AIDA_DataPoint::~AIDA_DataPoint()
{
  if ( m_dimension > 0 ) {
    for ( int iMeasurement = 0; iMeasurement < m_dimension; ++iMeasurement ) {
      delete m_measurements[ iMeasurement ];
    }
    delete [] m_measurements;
  }
}


AIDA::IMeasurement*
iAIDA::AIDA_DataPointSet_native::AIDA_DataPoint::coordinate( int c )
{
  if ( c < 0 || c >= m_dimension ) return 0;
  return ( m_measurements[ c ] );
}

const AIDA::IMeasurement*
iAIDA::AIDA_DataPointSet_native::AIDA_DataPoint::coordinate( int c ) const
{
  if ( c < 0 || c >= m_dimension ) return 0;
  return ( m_measurements[ c ] );
}
