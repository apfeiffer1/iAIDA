#include "RWNtuple.h"
#include "HBook.h"

iAIDA::AIDA_HBookStore::RWNtuple::RWNtuple( const std::string& memoryDirectory,
					     int id,  unsigned int numberOfVariables ):
  m_dir( memoryDirectory ),
  m_id( id ),
  m_cache( numberOfVariables, 0 ),
  m_initializedReading( false )
{}


bool
iAIDA::AIDA_HBookStore::RWNtuple::bindVariable( int )
{
  return true;
}


bool
iAIDA::AIDA_HBookStore::RWNtuple::clearBindings()
{
  return true;
}


bool
iAIDA::AIDA_HBookStore::RWNtuple::writeTupleRow( int )
{
  iAIDA::AIDA_HBookStore::HBook::changeDirectory( m_dir );
  iAIDA::AIDA_HBookStore::HBook::fillRWNtuple( m_id, m_cache );
  m_initializedReading = false;
  return true;
}


bool
iAIDA::AIDA_HBookStore::RWNtuple::readTupleRow( int rowNumber )
{
  iAIDA::AIDA_HBookStore::HBook::changeDirectory( m_dir );
  if ( ! m_initializedReading ) {
    iAIDA::AIDA_HBookStore::HBook::refreshRWNtuplePointers( m_id );
    m_initializedReading = true;
  }
  return iAIDA::AIDA_HBookStore::HBook::fastFillArrayFromRWNtupleRow( m_id, rowNumber + 1, &( m_cache[0] ) );
}

bool
iAIDA::AIDA_HBookStore::RWNtuple::reset()
{
  iAIDA::AIDA_HBookStore::HBook::changeDirectory( m_dir );
  iAIDA::AIDA_HBookStore::HBook::resetTuple( m_id );
  m_initializedReading = false;
  return true;
}


void*
iAIDA::AIDA_HBookStore::RWNtuple::variableAddress( int variableIndex )
{
  return &( m_cache[variableIndex] );
}

const void*
iAIDA::AIDA_HBookStore::RWNtuple::variableAddress( int variableIndex ) const
{
  return &( m_cache[variableIndex] );
}
