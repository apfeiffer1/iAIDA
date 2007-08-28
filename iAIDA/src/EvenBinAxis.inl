inline bool
iAIDA::AIDA_Histogram_native::EvenBinAxis::isFixedBinning() const
{
  return true;
}

inline double
iAIDA::AIDA_Histogram_native::EvenBinAxis::lowerEdge() const
{
  return m_lowEdge;
}

inline double
iAIDA::AIDA_Histogram_native::EvenBinAxis::upperEdge() const
{
  return m_highEdge;
}

inline int
iAIDA::AIDA_Histogram_native::EvenBinAxis::bins() const
{
  return m_bins;
}

inline double
iAIDA::AIDA_Histogram_native::EvenBinAxis::binWidth( int ) const
{
  return m_binSize;
}
