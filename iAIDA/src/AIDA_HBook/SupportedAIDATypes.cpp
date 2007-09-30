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
