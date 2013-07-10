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

#include "SupportedAIDATypes.h"

const iAIDA::AIDA_HBookStore::SupportedAIDATypes&
iAIDA::AIDA_HBookStore::SupportedAIDATypes::supportedTypes()
{
  static iAIDA::AIDA_HBookStore::SupportedAIDATypes st;
  return st;
}


iAIDA::AIDA_HBookStore::SupportedAIDATypes::SupportedAIDATypes()
{
  m_supportedTypes.insert( "IHistogram1D" );
  m_supportedTypes.insert( "IHistogram2D" );
  m_supportedTypes.insert( "IProfile1D" );
  m_supportedTypes.insert( "ITuple" );
}


bool
iAIDA::AIDA_HBookStore::SupportedAIDATypes::isTypeSupported( const std::string& type ) const
{
  if ( m_supportedTypes.find( type ) == m_supportedTypes.end() ) return false;
  else return true;
}
