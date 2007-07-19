// Emacs -*- C++ -*-

inline int
iAIDA::AIDA_Tuple_native::AIDA_TupleHeader::numberOfVariables() const
{
  return static_cast<int>( m_variableDescriptions.size() );
}


inline bool
iAIDA::AIDA_Tuple_native::AIDA_TupleHeader::setPathInStore( const std::string& path )
{
  if ( m_pathInStore.size() > 0 ) return false;
  m_pathInStore = path;
  return true;
}


inline const std::string&
iAIDA::AIDA_Tuple_native::AIDA_TupleHeader::pathInStore() const
{
  return m_pathInStore;
}


inline bool
iAIDA::AIDA_Tuple_native::AIDA_TupleHeader::setNumberOfRows( int rows )
{
  m_numberOfRows = rows;
  return true;
}

inline int
iAIDA::AIDA_Tuple_native::AIDA_TupleHeader::numberOfRows() const
{
  return m_numberOfRows;
}


inline bool
iAIDA::AIDA_Tuple_native::AIDA_TupleHeader::setCurrentRowNumber( int rowNumber )
{
  m_currentRowNumber = rowNumber;
  return true;
}


inline int
iAIDA::AIDA_Tuple_native::AIDA_TupleHeader::currentRowNumber() const
{
  return m_currentRowNumber;
}


inline bool
iAIDA::AIDA_Tuple_native::AIDA_TupleHeader::setOptions( const std::string& options )
{
  m_options = options;
  return true;
}


inline const std::string&
iAIDA::AIDA_Tuple_native::AIDA_TupleHeader::options() const
{
  return m_options;
}
