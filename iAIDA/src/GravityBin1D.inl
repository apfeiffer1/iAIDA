inline bool
iAIDA::AIDA_Histogram_native::GravityBin1D::fill( double weight,
						   double coord )
{
  m_entries++;
  m_sumOfWeights += weight;
  m_sumOfWeightTimesCoord += weight * coord;
  m_sumOfSquaredWeights += weight * weight;
  return true;
}


inline int
iAIDA::AIDA_Histogram_native::GravityBin1D::entries() const
{
  return m_entries;
}


inline double
iAIDA::AIDA_Histogram_native::GravityBin1D::height() const
{
  return m_sumOfWeights;
}


inline double
iAIDA::AIDA_Histogram_native::GravityBin1D::errorSquared() const
{
  return m_sumOfSquaredWeights;
}
