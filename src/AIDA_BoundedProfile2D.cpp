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

#include "AIDA_BoundedProfile2D.h"
#include <typeinfo>

iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile2D::AIDA_BoundedProfile2D( const std::string& title,
									     int numberOfBinsX,
									     double lowEdgeX,
									     double highEdgeX,
									     int numberOfBinsY,
									     double lowEdgeY,
									     double highEdgeY,
									     double lowBound,
									     double highBound ):
  iAIDA::AIDA_Histogram_native::AIDA_Profile2D( title, numberOfBinsX, lowEdgeX, highEdgeX, numberOfBinsY, lowEdgeY, highEdgeY ),
  m_extraEntries( 0 ),
  m_lowBound( lowBound ),
  m_highBound( highBound )
{}


iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile2D::AIDA_BoundedProfile2D( const std::string& title,
									     const std::vector< double >& edgesX,
									     const std::vector< double >& edgesY,
									     double lowBound,
									     double highBound ):
  iAIDA::AIDA_Histogram_native::AIDA_Profile2D( title, edgesX, edgesY ),
  m_extraEntries( 0 ),
  m_lowBound( lowBound ),
  m_highBound( highBound )
{}


iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile2D::AIDA_BoundedProfile2D( const AIDA_BoundedProfile2D& h ):
  iAIDA::AIDA_Histogram_native::AIDA_Profile2D( static_cast< const AIDA_Profile2D& >( h ) ),
  m_extraEntries( h.m_extraEntries ),
  m_lowBound( h.m_lowBound ),
  m_highBound( h.m_highBound )
{}


bool
iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile2D::reset()
{
  m_extraEntries = 0;
  return this->iAIDA::AIDA_Histogram_native::AIDA_Profile2D::reset();
}


int
iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile2D::extraEntries() const
{
  return m_extraEntries + this->iAIDA::AIDA_Histogram_native::AIDA_Profile2D::extraEntries();
}


bool
iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile2D::fill( double x, double y, double z, double weight )
{
  if ( z < m_lowBound || z > m_highBound ) {
    ++m_extraEntries;
    return true;
  }
  else return this->iAIDA::AIDA_Histogram_native::AIDA_Profile2D::fill( x, y, z, weight );
}


bool
iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile2D::add( const AIDA::IProfile2D & h )
{
  if ( ! ( this->iAIDA::AIDA_Histogram_native::AIDA_Profile2D::add( h ) ) ) return false;
  try {
    const iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile2D& hb = dynamic_cast< const iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile2D& >( h );
    m_extraEntries += hb.m_extraEntries;
  }
  catch( std::bad_cast ) {
  }
  return true;
}
