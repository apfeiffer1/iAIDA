#include "VariableBinAxis.h"
#include "AIDA/IConstants.h"

#ifdef NONUMERICLIMITS
static const double positive_infinity = 1.7976931348623157e+308;
static const double negative_infinity = -1.7976931348623157e+308;
#else
#include <limits>
static const double positive_infinity = std::numeric_limits<double>::max();
static const double negative_infinity = -positive_infinity;
#endif

#include <algorithm>

iAIDA::AIDA_Histogram_native::VariableBinAxis::VariableBinAxis( const std::vector< double> & edges ) : 
  m_binEdges( edges )
{

  // LM :  sort vector of edges in ascending order
  std::sort(m_binEdges.begin(), m_binEdges.end() );

  double currentEdge = m_binEdges[ 0 ];
  m_binUpperEdges.insert( std::make_pair( currentEdge, static_cast< int >( AIDA::IAxis::UNDERFLOW_BIN ) ) );
  for ( unsigned int i = 1; i < m_binEdges.size(); ++i ) {
    m_binUpperEdges.insert( std::make_pair( m_binEdges[ i ], static_cast< int >( i - 1 ) ) );
  }
  m_binUpperEdges.insert( std::make_pair( positive_infinity, static_cast< int >( AIDA::IAxis::OVERFLOW_BIN ) ) );
}

double
iAIDA::AIDA_Histogram_native::VariableBinAxis::binLowerEdge( int index ) const
{
  if ( index >= 0 ) {
    return m_binEdges[ index ];
  }
  else if ( index == static_cast< int >( AIDA::IAxis::UNDERFLOW_BIN ) ) return negative_infinity;
  else return m_binEdges.back();
}

double
iAIDA::AIDA_Histogram_native::VariableBinAxis::binUpperEdge( int index ) const
{
  if ( index >= 0 ) {
    return m_binEdges[ index + 1];
  }
  else if ( index == static_cast< int >( AIDA::IAxis::UNDERFLOW_BIN ) ) return m_binEdges.front();
  else return positive_infinity;
}

double
iAIDA::AIDA_Histogram_native::VariableBinAxis::binWidth( int index ) const
{
  if ( index >= 0 ) {
    return m_binEdges[ index + 1 ] - m_binEdges[ index ];
  }
  else {
    return positive_infinity;
  };
}
