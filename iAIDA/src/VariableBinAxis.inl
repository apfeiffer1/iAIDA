inline bool
iAIDA::AIDA_Histogram_native::VariableBinAxis::isFixedBinning() const
{
  return false;
}

inline double
iAIDA::AIDA_Histogram_native::VariableBinAxis::lowerEdge() const
{
  return m_binEdges.front();
}

inline double
iAIDA::AIDA_Histogram_native::VariableBinAxis::upperEdge() const
{
  return m_binEdges.back();
}

inline int
iAIDA::AIDA_Histogram_native::VariableBinAxis::bins() const
{
  return m_binEdges.size() - 1;
}

inline int
iAIDA::AIDA_Histogram_native::VariableBinAxis::coordToIndex( double coord ) const
{
  return m_binUpperEdges.upper_bound( coord )->second;
}
