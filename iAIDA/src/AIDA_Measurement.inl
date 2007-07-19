inline double
iAIDA::AIDA_DataPointSet_native::AIDA_Measurement::value() const
{
  return m_value;
}

inline double
iAIDA::AIDA_DataPointSet_native::AIDA_Measurement::errorPlus() const
{
  return m_errorPlus;
}

inline double
iAIDA::AIDA_DataPointSet_native::AIDA_Measurement::errorMinus() const
{
  return m_errorMinus;
}

inline bool
iAIDA::AIDA_DataPointSet_native::AIDA_Measurement::setValue( double v )
{
  m_value = v;
  return true;
}

inline bool
iAIDA::AIDA_DataPointSet_native::AIDA_Measurement::setErrorPlus( double ep )
{
  m_errorPlus = ep;
return true;
}

inline bool
iAIDA::AIDA_DataPointSet_native::AIDA_Measurement::setErrorMinus( double em )
{
  m_errorMinus = em;
return true;
}
