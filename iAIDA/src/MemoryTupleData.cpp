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

#include "AIDA_MemoryStore/MemoryTupleData.h"
#include "AIDA_Dev/ITupleVariableDescription.h"

iAIDA::AIDA_MemoryStore::MemoryTupleData::MemoryTupleData():
  m_title( "" ),
  m_variables()
{
  m_variables.reserve( 1000 );
}


iAIDA::AIDA_MemoryStore::MemoryTupleData::~MemoryTupleData()
{
  for ( unsigned int i = 0; i < m_variables.size(); ++i ) {
    delete m_variables[i];
  }

  for( std::map< int, std::vector< iAIDA::AIDA_MemoryStore::MemoryTupleData* > >::iterator i = m_tupleVariableData.begin();
       i != m_tupleVariableData.end(); ++i ) {
    std::vector< iAIDA::AIDA_MemoryStore::MemoryTupleData* >& vec = i->second;
    for ( std::vector< iAIDA::AIDA_MemoryStore::MemoryTupleData* >::iterator j = vec.begin(); j != vec.end(); ++j ) {
      if ( *j ) delete *j;
    }
  }
}
