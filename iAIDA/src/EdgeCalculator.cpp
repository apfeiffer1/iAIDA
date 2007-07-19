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
