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

#ifndef IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_SUBTUPLE_H
#define IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_SUBTUPLE_H

#include "ISubTuple.h"
#include "AIDA_Dev/IDevTuple.h"

namespace iAIDA {
  namespace AIDA_HBookStore {

class SubTuple : virtual public ISubTuple
{
public:
  SubTuple( AIDA::Dev::IDevTuple* tuple,
	    ISubTuple* subTuple,
            int maxLength ) :
    m_tuple( tuple ),
    m_subTuple( subTuple ),
    m_maxLength( maxLength ),
    m_firstElement( 0 )
  {}
  ~SubTuple() {}
  bool setFirstElement( unsigned int i )
  {
    m_firstElement = i * m_maxLength;
    return true;
  }

  bool writeTupleRow( int rowNumber )
  {
    if ( rowNumber < 0 || rowNumber >= m_maxLength ) return false;
    if ( ! m_subTuple->setFirstElement( m_firstElement + rowNumber + 1 ) ) return false;
    m_tuple->reset();
    return true;
  }

  bool readTupleRow( int rowNumber )
  {
    if ( rowNumber < 0 || rowNumber >= m_maxLength ) return false;
    return m_subTuple->setFirstElement( m_firstElement + rowNumber );
  }

  bool reset()
  {
    return m_subTuple->setFirstElement( m_firstElement );
  }

  void* variableAddress( int ) {
    return m_tuple;
  }

  const void* variableAddress( int ) const {
    return m_tuple;
  }

private:
  AIDA::Dev::IDevTuple* m_tuple;
  ISubTuple* m_subTuple;
  int m_maxLength;
  unsigned int m_firstElement;
};

  }
}

#endif
