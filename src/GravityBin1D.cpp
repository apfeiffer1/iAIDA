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

#include "GravityBin1D.h"
#include <cmath>

iAIDA::AIDA_Histogram_native::GravityBin1D::GravityBin1D():
  m_entries( 0 ),
  m_sumOfWeights( 0 ),
  m_sumOfWeightTimesCoord( 0 ),
  m_sumOfSquaredWeights( 0 )
{}


bool
iAIDA::AIDA_Histogram_native::GravityBin1D::reset()
{
  m_sumOfSquaredWeights = m_sumOfWeightTimesCoord = m_sumOfWeights = m_entries = 0;
  return true;
}

bool
iAIDA::AIDA_Histogram_native::GravityBin1D::scale( double factor )
{
  m_sumOfWeights *= factor;
  m_sumOfWeightTimesCoord *= factor;
  m_sumOfSquaredWeights *= factor * factor;
  return true;
}

bool
iAIDA::AIDA_Histogram_native::GravityBin1D::increment( const iAIDA::AIDA_Histogram_native::GravityBin1D& otherBin )
{
  m_entries += otherBin.m_entries;
  m_sumOfWeights += otherBin.m_sumOfWeights;
  m_sumOfWeightTimesCoord += otherBin.m_sumOfWeightTimesCoord;
  m_sumOfSquaredWeights += otherBin.m_sumOfSquaredWeights;
  return true;
}

bool
iAIDA::AIDA_Histogram_native::GravityBin1D::increment( int extraEntries,
							double extraHeight,
							double extraError,
							double extraCentre )
{
  m_entries += extraEntries;
  m_sumOfWeights += extraHeight;
  m_sumOfWeightTimesCoord += extraCentre * extraHeight;
  m_sumOfSquaredWeights += extraError * extraError;
  return true;
}

bool
iAIDA::AIDA_Histogram_native::GravityBin1D::set( int entriesNew,
						  double heightNew,
						  double errorNew,
						  double centreNew )
{
  m_entries = entriesNew;
  m_sumOfWeights = heightNew;
  m_sumOfWeightTimesCoord = centreNew * heightNew;
  m_sumOfSquaredWeights = errorNew * errorNew;
  return true;
}

double
iAIDA::AIDA_Histogram_native::GravityBin1D::error() const
{
  return std::sqrt( m_sumOfSquaredWeights );
}


double
iAIDA::AIDA_Histogram_native::GravityBin1D::centreOfGravityX() const
{
    return ( ( m_sumOfWeights == 0 ) ? 0 : ( m_sumOfWeightTimesCoord / m_sumOfWeights ) );
}
