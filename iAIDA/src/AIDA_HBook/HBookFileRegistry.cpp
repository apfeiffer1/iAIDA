#include "HBookFileRegistry.h"

pi::AIDA_HBookStore::HBookFileRegistry&
pi::AIDA_HBookStore::HBookFileRegistry::theRegistry()
{
  static pi::AIDA_HBookStore::HBookFileRegistry registry;
  return registry;
}


int
pi::AIDA_HBookStore::HBookFileRegistry::nextAvailableLUN( const std::string& fileName )
{
  if ( m_fileNames.find( fileName ) != m_fileNames.end() ) return 0;

  int i = 1;
  while ( i < static_cast<int>( m_allocatedLUNs.size() ) + 1 ) {
    if ( m_allocatedLUNs.find( i ) == m_allocatedLUNs.end() ) break;
    ++i;
  }

  m_allocatedLUNs.insert( std::make_pair( i, fileName ) );
  m_fileNames.insert( std::make_pair( fileName, i ) );
  return i;
}


void
pi::AIDA_HBookStore::HBookFileRegistry::releaseLUN( int lun )
{
  std::map< int, std::string >::const_iterator iLun = m_allocatedLUNs.find( lun );
  if ( iLun != m_allocatedLUNs.end() ) {
    std::string fileName = iLun->second;
    m_allocatedLUNs.erase( lun );
    m_fileNames.erase( fileName );
  }
}
