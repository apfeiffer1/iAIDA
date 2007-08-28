#include "EvenBinAxis.h"
#include <cmath>

#ifdef NONUMERICLIMITS
static const double positive_infinity = 1.7976931348623157e+308;
static const double negative_infinity = -1.7976931348623157e+308;
#else
#include <limits>
static const double positive_infinity = std::numeric_limits<double>::max();
static const double negative_infinity = -positive_infinity;
#endif

iAIDA::AIDA_Histogram_native::EvenBinAxis::EvenBinAxis( int nBins,
							 double lowEdge,
							 double highEdge ):
  m_bins( nBins ),
  m_lowEdge( lowEdge ),
  m_highEdge( highEdge ),
  m_binSize( ( m_highEdge - m_lowEdge ) / m_bins )
{}

double
iAIDA::AIDA_Histogram_native::EvenBinAxis::binLowerEdge( int index ) const
{
  if ( index >= 0 && index <= m_bins ) {
    return m_lowEdge + index * m_binSize;
  }
  if ( index > m_bins || index == static_cast< int >( AIDA::IAxis::OVERFLOW_BIN ) ) return m_highEdge;
  else return negative_infinity;
}

double
iAIDA::AIDA_Histogram_native::EvenBinAxis::binUpperEdge( int index ) const
{
  if ( index >= 0 && index < m_bins ) {
    return m_lowEdge + ( index + 1 ) * m_binSize;
  }
  if ( index > m_bins || index == static_cast< int >( AIDA::IAxis::OVERFLOW_BIN ) ) return positive_infinity;
  else return m_lowEdge;
}

int
iAIDA::AIDA_Histogram_native::EvenBinAxis::coordToIndex( double coord ) const
{
  if ( coord < m_lowEdge ) return static_cast< int >( AIDA::IAxis::UNDERFLOW_BIN );
  else if ( coord >= m_highEdge ) return static_cast< int >( AIDA::IAxis::OVERFLOW_BIN );
  else return static_cast< int >( std::floor( ( coord - m_lowEdge ) / m_binSize ) );
}
