// Emacs -*- C++ -*-
inline const std::string&
iAIDA::AIDA_Tuple_native::AIDA_TupleVariableDescription::variableName() const
{
  return m_name;
}


inline bool
iAIDA::AIDA_Tuple_native::AIDA_TupleVariableDescription::setVariableName( const std::string& name )
{
  if ( m_name.size() > 0 || name.size() == 0 ) return false;
  m_name = name;
  return true;
}


inline const std::string&
iAIDA::AIDA_Tuple_native::AIDA_TupleVariableDescription::variableType() const
{
  return m_type;
}


inline bool
iAIDA::AIDA_Tuple_native::AIDA_TupleVariableDescription::setVariableType( const std::string& type )
{
  if ( m_type.size() > 0 || type.size() == 0 ) return false;
  m_type = type;
  return true;
}


inline int
iAIDA::AIDA_Tuple_native::AIDA_TupleVariableDescription::numberOfVariables() const
{
  return static_cast<int>( m_variableDescriptions.size() );
}
