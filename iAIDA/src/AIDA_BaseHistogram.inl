inline int
iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::dimension() const
{
  return m_dimension;
}

inline bool
iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::isUpToDate() const
{
  return m_upToDate;
}

inline void
iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::setUpToDate(bool isUpToDate)
{
  m_upToDate=isUpToDate;
}

inline const std::string&
iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::userLevelClassType() const
{
  return m_classType;
}
