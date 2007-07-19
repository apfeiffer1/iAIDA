inline int
iAIDA::AIDA_Histogram_native::ProfileBin1D::entries() const
{
  return m_entries;
}

inline bool
iAIDA::AIDA_Histogram_native::ProfileBin1D::reset()
{
  m_sumOfWeights = m_sumOfWeightTimesExtraCoordinate = m_sumOfWeightTimesSquaredExtraCoordinate = m_sumOfWeightTimesCoordinate = m_entries = 0;
  return true;
}


inline bool
iAIDA::AIDA_Histogram_native::ProfileBin1D::scale( double scaleFactor )
{
  m_sumOfWeights *= scaleFactor;
  m_sumOfWeightTimesExtraCoordinate *= scaleFactor;
  m_sumOfWeightTimesSquaredExtraCoordinate *= scaleFactor;
  m_sumOfWeightTimesCoordinate *= scaleFactor;
  return true;
}


inline bool
iAIDA::AIDA_Histogram_native::ProfileBin1D::fill( double weight,
						   double x,
						   double y )
{
  ++m_entries;
  m_sumOfWeights += weight;
  m_sumOfWeightTimesExtraCoordinate += weight * y;
  m_sumOfWeightTimesSquaredExtraCoordinate += weight * y * y;
  m_sumOfWeightTimesCoordinate += weight * x;
  return true;
}

inline double
iAIDA::AIDA_Histogram_native::ProfileBin1D::sumOfWeights() const
{
  return m_sumOfWeights;
}
