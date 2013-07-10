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

#include "GravityBin3D.h"

iAIDA::AIDA_Histogram_native::GravityBin3D::GravityBin3D():
  iAIDA::AIDA_Histogram_native::GravityBin2D(),
  m_sumOfWeightTimesCoord( 0 )
{}


bool
iAIDA::AIDA_Histogram_native::GravityBin3D::reset()
{
  m_sumOfWeightTimesCoord = 0;
  return iAIDA::AIDA_Histogram_native::GravityBin2D::reset();
}


bool
iAIDA::AIDA_Histogram_native::GravityBin3D::scale( double factor )
{
  m_sumOfWeightTimesCoord *= factor;
  return iAIDA::AIDA_Histogram_native::GravityBin2D::scale( factor );
}


bool
iAIDA::AIDA_Histogram_native::GravityBin3D::increment( const iAIDA::AIDA_Histogram_native::GravityBin3D& otherBin )
{
  m_sumOfWeightTimesCoord += otherBin.m_sumOfWeightTimesCoord;
  return iAIDA::AIDA_Histogram_native::GravityBin2D::increment( static_cast< const iAIDA::AIDA_Histogram_native::GravityBin2D& >( otherBin ) );
}


bool
iAIDA::AIDA_Histogram_native::GravityBin3D::increment( int extraEntries,
							double extraHeight,
							double extraError,
							double extraCentreX,
							double extraCentreY,
							double extraCentreZ )
{
  m_sumOfWeightTimesCoord += extraCentreZ * extraHeight;
  return iAIDA::AIDA_Histogram_native::GravityBin2D::increment( extraEntries, extraHeight, extraError, extraCentreX, extraCentreY );
}


bool
iAIDA::AIDA_Histogram_native::GravityBin3D::set( int entriesNew,
						  double heightNew,
						  double errorNew,
						  double centreXNew,
						  double centreYNew,
						  double centreZNew )
{
  m_sumOfWeightTimesCoord = centreZNew * heightNew;
  return iAIDA::AIDA_Histogram_native::GravityBin2D::set( entriesNew, heightNew, errorNew, centreXNew, centreYNew );
}


double
iAIDA::AIDA_Histogram_native::GravityBin3D::centreOfGravityZ() const
{
  const double theHeight = height();
  if ( theHeight == 0 ) {
    return 0;
  }
  else {
    return ( m_sumOfWeightTimesCoord / theHeight );
  }
}
