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

#include "EdgeCalculator.h"
#include <cmath>

bool
iAIDA::AIDA_Histogram_native::EdgeCalculator::calculateEdges( double edgeLow,
							       double edgeHigh,
							       int bins,
							       double& newEdgeLow,
							       double& newEdgeHigh ) const
{
  if ( bins == 0 ) return false;
  if ( edgeHigh <= edgeLow ) return false;
  const double binSize = ( edgeHigh - edgeLow ) / bins;
  const double magnitude = std::floor( std::log10( binSize ) );
  const double precision = std::pow( static_cast<double>(10), magnitude );

  newEdgeHigh = precision * ( std::floor( edgeHigh / precision ) + 1 );
  newEdgeLow = precision * std::floor( edgeLow / precision );

  return true;
}
