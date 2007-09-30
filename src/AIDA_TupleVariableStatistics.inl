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

// Emacs -*- C++ -*-

inline double
iAIDA::AIDA_Tuple_native::AIDA_TupleVariableStatistics::min() const
{
  return m_min;
}

inline double
iAIDA::AIDA_Tuple_native::AIDA_TupleVariableStatistics::max() const
{
  return m_max;
}

inline double
iAIDA::AIDA_Tuple_native::AIDA_TupleVariableStatistics::mean() const
{
  return ( ( m_entries == 0 ) ? 0 : m_sumOfValues / m_entries );
}

inline int
iAIDA::AIDA_Tuple_native::AIDA_TupleVariableStatistics::entries() const
{
  return m_entries;
}

inline bool
iAIDA::AIDA_Tuple_native::AIDA_TupleVariableStatistics::reset()
{
  m_min = m_max = m_sumOfValues = m_sumOfValuesSquared = m_entries = 0;
  return true;
}

inline bool
iAIDA::AIDA_Tuple_native::AIDA_TupleVariableStatistics::addEntry( double value )
{
  m_sumOfValues += value;
  m_sumOfValuesSquared += value * value;
  if ( m_entries > 0 ) {
    if ( value >= m_max ) m_max = value;
    if ( value <= m_min ) m_min = value;
  }
  else {
    m_min = m_max = value;
  }
  ++m_entries;
  return true;
}
