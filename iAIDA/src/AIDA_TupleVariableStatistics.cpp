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

