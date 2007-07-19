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
