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

#include "ProfileBin2D.h"

iAIDA::AIDA_Histogram_native::ProfileBin2D::ProfileBin2D():
  iAIDA::AIDA_Histogram_native::ProfileBin1D(),
  m_sumOfWeightTimesCoordinate( 0 )
{}


bool
iAIDA::AIDA_Histogram_native::ProfileBin2D::reset()
{
  m_sumOfWeightTimesCoordinate = 0;
  return iAIDA::AIDA_Histogram_native::ProfileBin1D::reset();
}


bool
iAIDA::AIDA_Histogram_native::ProfileBin2D::scale( double factor )
{
  m_sumOfWeightTimesCoordinate *= factor;
  return iAIDA::AIDA_Histogram_native::ProfileBin1D::scale( factor );
}


bool
iAIDA::AIDA_Histogram_native::ProfileBin2D::increment( const iAIDA::AIDA_Histogram_native::ProfileBin2D& otherBin )
{
  m_sumOfWeightTimesCoordinate += otherBin.m_sumOfWeightTimesCoordinate;
  return iAIDA::AIDA_Histogram_native::ProfileBin1D::increment( static_cast< const iAIDA::AIDA_Histogram_native::ProfileBin1D& >( otherBin ) );
}


bool
iAIDA::AIDA_Histogram_native::ProfileBin2D::increment( int extraEntries,
							double extraHeight,
							double extraError,
							double extraSpread,
							double extraCentreX,
							double extraCentreY )
{
  iAIDA::AIDA_Histogram_native::ProfileBin2D temporaryBin;
  temporaryBin.set( extraEntries, extraHeight, extraError, extraSpread, extraCentreX, extraCentreY );
  return this->increment( temporaryBin );
}


bool
iAIDA::AIDA_Histogram_native::ProfileBin2D::set( int entriesNew,
						  double heightNew,
						  double errorNew,
						  double spreadNew,
						  double centreXNew,
						  double centreYNew )
{
  bool iret = iAIDA::AIDA_Histogram_native::ProfileBin1D::set( entriesNew, heightNew, errorNew, spreadNew, centreXNew );
  m_sumOfWeightTimesCoordinate = centreYNew * sumOfWeights(); 
  return iret; 
}


double
iAIDA::AIDA_Histogram_native::ProfileBin2D::centreOfGravityY() const
{
  const double theHeight = sumOfWeights();
  if ( theHeight == 0 ) {
    return 0;
  }
  else {
    return ( m_sumOfWeightTimesCoordinate / theHeight );
  }
}
