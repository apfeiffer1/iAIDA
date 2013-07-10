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
