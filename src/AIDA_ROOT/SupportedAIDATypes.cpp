#include "SupportedAIDATypes.h"

const pi::AIDA_RootStore::SupportedAIDATypes&
pi::AIDA_RootStore::SupportedAIDATypes::supportedTypes()
{
  static pi::AIDA_RootStore::SupportedAIDATypes st;
  return st;
}


pi::AIDA_RootStore::SupportedAIDATypes::SupportedAIDATypes()
{
  m_supportedTypes.insert( "IHistogram1D" );
  m_supportedTypes.insert( "IHistogram2D" );
  m_supportedTypes.insert( "IHistogram3D" );
  m_supportedTypes.insert( "IProfile1D" );
  m_supportedTypes.insert( "IProfile2D" );
  m_supportedTypes.insert( "ITuple" );
}


bool
pi::AIDA_RootStore::SupportedAIDATypes::isTypeSupported( const std::string& type ) const
{
  if ( m_supportedTypes.find( type ) == m_supportedTypes.end() ) return false;
  else return true;
}
