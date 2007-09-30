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
