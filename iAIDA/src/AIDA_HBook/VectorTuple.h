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

#ifndef IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_VECTORTUPLE_H
#define IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_VECTORTUPLE_H

#include "ISubTuple.h"
#include <vector>

namespace iAIDA {
  namespace AIDA_HBookStore {

template< class T > class VectorTuple : virtual public ISubTuple
{
public:
  VectorTuple( unsigned int numberOfElements,
	       int bufferSize ):
    m_data( numberOfElements ),
    m_bufferSize( bufferSize ),
    m_firstElement( 0 )
  {}
  ~VectorTuple() {}

  T* startAddress() { return &( m_data.front() ); }
  const T* startAddress() const { return &( m_data.front() ); }
  bool setFirstElement( unsigned int i )
  {
    const unsigned int fe = i * m_bufferSize;
    if ( fe > m_data.size() ) return false;
    m_firstElement = fe; return true;
  }

  bool writeTupleRow( int rowNumber )
  {
    if ( rowNumber < 0 || rowNumber >= m_bufferSize ) return false;
    const unsigned int i = rowNumber + m_firstElement;
    if ( i >= m_data.size() ) return false;
    m_data[i] = m_cache; return true;
  }
  bool readTupleRow( int rowNumber )
  {
    if ( rowNumber < 0 || rowNumber >= m_bufferSize ) return false;
    const unsigned int i = rowNumber + m_firstElement;
    if ( i >= m_data.size() ) return false;
    m_cache = m_data[i]; return true;
  }
  bool reset() { return true; }
  void* variableAddress( int ) { return &m_cache; }
  const void* variableAddress( int ) const { return &m_cache; }

private:
  std::vector< T > m_data;
  int              m_bufferSize;
  unsigned int     m_firstElement;
  T                m_cache;
};

  }
}

#endif
