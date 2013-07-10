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

#include "AIDA_BoundedProfile1D.h"
#include <typeinfo>

iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile1D::AIDA_BoundedProfile1D( const std::string& title,
									     int numberOfBins,
									     double lowEdge,
									     double highEdge,
									     double lowBound,
									     double highBound ):
  iAIDA::AIDA_Histogram_native::AIDA_Profile1D( title, numberOfBins, lowEdge, highEdge ),
  m_extraEntries( 0 ),
  m_lowBound( lowBound ),
  m_highBound( highBound )
{}


iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile1D::AIDA_BoundedProfile1D( const std::string& title,
									     const std::vector< double >& edges,
									     double lowBound,
									     double highBound ):
  iAIDA::AIDA_Histogram_native::AIDA_Profile1D( title, edges ),
  m_extraEntries( 0 ),
  m_lowBound( lowBound ),
  m_highBound( highBound )
{}


iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile1D::AIDA_BoundedProfile1D( const AIDA_BoundedProfile1D& h ):
  iAIDA::AIDA_Histogram_native::AIDA_Profile1D( static_cast< const AIDA_Profile1D& >( h ) ),
  m_extraEntries( h.m_extraEntries ),
  m_lowBound( h.m_lowBound ),
  m_highBound( h.m_highBound )
{}


bool
iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile1D::reset()
{
  m_extraEntries = 0;
  return this->iAIDA::AIDA_Histogram_native::AIDA_Profile1D::reset();
}


int
iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile1D::extraEntries() const
{
  return m_extraEntries + this->iAIDA::AIDA_Histogram_native::AIDA_Profile1D::extraEntries();
}


bool
iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile1D::fill( double x, double y, double weight )
{
  if ( y < m_lowBound || y > m_highBound ) {
    ++m_extraEntries;
    return true;
  }
  else return this->iAIDA::AIDA_Histogram_native::AIDA_Profile1D::fill( x, y, weight );
}


bool
iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile1D::add( const AIDA::IProfile1D & h )
{
  if ( ! ( this->iAIDA::AIDA_Histogram_native::AIDA_Profile1D::add( h )  ) ) return false;
  try {
    const iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile1D& hb = dynamic_cast< const iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile1D& >( h );
    m_extraEntries += hb.m_extraEntries;
  }
  catch( std::bad_cast ) {
  }
  return true;
}
