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
