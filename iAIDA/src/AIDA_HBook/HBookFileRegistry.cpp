#include "HBookFileRegistry.h"

iAIDA::AIDA_HBookStore::HBookFileRegistry&
iAIDA::AIDA_HBookStore::HBookFileRegistry::theRegistry()
{
  static iAIDA::AIDA_HBookStore::HBookFileRegistry registry;
  return registry;
}


int
iAIDA::AIDA_HBookStore::HBookFileRegistry::nextAvailableLUN( const std::string& fileName )
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
iAIDA::AIDA_HBookStore::HBookFileRegistry::releaseLUN( int lun )
{
  std::map< int, std::string >::const_iterator iLun = m_allocatedLUNs.find( lun );
  if ( iLun != m_allocatedLUNs.end() ) {
    std::string fileName = iLun->second;
    m_allocatedLUNs.erase( lun );
    m_fileNames.erase( fileName );
  }
}
