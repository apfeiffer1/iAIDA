#include "SupportedAIDATypes.h"

const pi::AIDA_HBookStore::SupportedAIDATypes&
pi::AIDA_HBookStore::SupportedAIDATypes::supportedTypes()
{
  static pi::AIDA_HBookStore::SupportedAIDATypes st;
  return st;
}


pi::AIDA_HBookStore::SupportedAIDATypes::SupportedAIDATypes()
{
  m_supportedTypes.insert( "IHistogram1D" );
  m_supportedTypes.insert( "IHistogram2D" );
  m_supportedTypes.insert( "IProfile1D" );
  m_supportedTypes.insert( "ITuple" );
}


bool
pi::AIDA_HBookStore::SupportedAIDATypes::isTypeSupported( const std::string& type ) const
{
  if ( m_supportedTypes.find( type ) == m_supportedTypes.end() ) return false;
  else return true;
}
