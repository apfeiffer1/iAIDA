#include "AIDA_TupleVariableStatistics.h"
#include <cmath>

iAIDA::AIDA_Tuple_native::AIDA_TupleVariableStatistics::AIDA_TupleVariableStatistics():
  m_min( 0 ),
  m_max( 0 ),
  m_sumOfValues( 0 ),
  m_sumOfValuesSquared( 0 ),
  m_entries( 0 )
{}


bool
iAIDA::AIDA_Tuple_native::AIDA_TupleVariableStatistics::setStatistics( double valueMin,
									double valueMax,
									double valueMean,
									double valueRms,
									int numberOfEntries )
{
  m_min = valueMin;
  m_max = valueMax;
  m_sumOfValues = valueMean * numberOfEntries;
  m_sumOfValuesSquared = ( valueMean * valueMean - valueRms * valueRms ) * numberOfEntries;
  m_entries = numberOfEntries;
  return true;
}

double
iAIDA::AIDA_Tuple_native::AIDA_TupleVariableStatistics::rms() const
{
  if ( m_entries == 0 ) return 0;
  const double meanx = mean();
  return std::sqrt( std::abs( m_sumOfValuesSquared / m_entries - meanx*meanx ) );
}

