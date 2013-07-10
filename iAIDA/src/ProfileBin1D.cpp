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

#include "ProfileBin1D.h"
#include <cmath>


iAIDA::AIDA_Histogram_native::ProfileBin1D::ProfileBin1D():
  m_entries( 0 ),
  m_sumOfWeights( 0 ),
  m_sumOfWeightTimesExtraCoordinate( 0 ),
  m_sumOfWeightTimesSquaredExtraCoordinate( 0 ),
  m_sumOfWeightTimesCoordinate( 0 )
{}


double
iAIDA::AIDA_Histogram_native::ProfileBin1D::value() const
{
  return ( ( m_sumOfWeights ==0 )? 0 : ( m_sumOfWeightTimesExtraCoordinate / m_sumOfWeights ) );
}


double
iAIDA::AIDA_Histogram_native::ProfileBin1D::error() const
{
  if ( m_sumOfWeights == 0 ) return 0;
  const double s = spread();
  if ( s == 0 ) return std::sqrt( m_sumOfWeightTimesExtraCoordinate / (m_sumOfWeights * m_sumOfWeights) );
  else return ( s / std::sqrt( m_sumOfWeights ) );
}


double
iAIDA::AIDA_Histogram_native::ProfileBin1D::spread() const
{
  if ( m_sumOfWeights == 0 ) return 0;
  const double hl = m_sumOfWeightTimesExtraCoordinate / m_sumOfWeights;
  const double hl2 = hl * hl;
  double s = 0; 
  if (m_entries > 1)  
    s = std::abs( m_sumOfWeightTimesSquaredExtraCoordinate / m_sumOfWeights - hl2 );
  if ( s < 1.0e-15 ) return std::sqrt( std::abs( hl ) );
  else return std::sqrt( s );
}

bool
iAIDA::AIDA_Histogram_native::ProfileBin1D::increment( const iAIDA::AIDA_Histogram_native::ProfileBin1D & bin )
{
  m_entries += bin.m_entries;
  m_sumOfWeights += bin.m_sumOfWeights;
  m_sumOfWeightTimesExtraCoordinate += bin.m_sumOfWeightTimesExtraCoordinate;
  m_sumOfWeightTimesSquaredExtraCoordinate += bin.m_sumOfWeightTimesSquaredExtraCoordinate;
  m_sumOfWeightTimesCoordinate += bin.m_sumOfWeightTimesCoordinate;
  return true;
}


bool
iAIDA::AIDA_Histogram_native::ProfileBin1D::increment( int extraEntries,
							double extraValue,
							double extraError,
							double extraSpread,
							double extraCentre )
{
  iAIDA::AIDA_Histogram_native::ProfileBin1D temporaryBin;
  temporaryBin.set( extraEntries, extraValue, extraError, extraSpread, extraCentre );
  return this->increment( temporaryBin );
}


bool
iAIDA::AIDA_Histogram_native::ProfileBin1D::set( int newEntries,
						  double newValue,
						  double newError,
						  double newSpread,
						  double newCentre )
{
  m_entries = newEntries;
  m_sumOfWeights = m_sumOfWeightTimesExtraCoordinate = m_sumOfWeightTimesSquaredExtraCoordinate = m_sumOfWeightTimesCoordinate = 0;
  if ( m_entries !=0 && newValue != 0 && newError != 0 && newSpread != 0 ) {
     m_sumOfWeights = newSpread / newError;
     m_sumOfWeights *= m_sumOfWeights;
     m_sumOfWeightTimesExtraCoordinate = newValue * m_sumOfWeights;
     m_sumOfWeightTimesSquaredExtraCoordinate = m_sumOfWeightTimesExtraCoordinate / m_sumOfWeights;
     m_sumOfWeightTimesSquaredExtraCoordinate *= m_sumOfWeightTimesSquaredExtraCoordinate;
     m_sumOfWeightTimesSquaredExtraCoordinate += newSpread * newSpread;
     m_sumOfWeightTimesSquaredExtraCoordinate *= m_sumOfWeights;
     m_sumOfWeightTimesCoordinate = newCentre * m_sumOfWeights;
  }
  return true;
}


double
iAIDA::AIDA_Histogram_native::ProfileBin1D::centreOfGravityX() const
{
    return ( ( m_sumOfWeights == 0 ) ? 0 : ( m_sumOfWeightTimesCoordinate / m_sumOfWeights ) );
}
