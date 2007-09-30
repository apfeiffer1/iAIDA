#include "SupportedAIDATypes.h"

const iAIDA::AIDA_RootStore::SupportedAIDATypes&
iAIDA::AIDA_RootStore::SupportedAIDATypes::supportedTypes()
{
  static iAIDA::AIDA_RootStore::SupportedAIDATypes st;
  return st;
}


iAIDA::AIDA_RootStore::SupportedAIDATypes::SupportedAIDATypes()
{
  m_supportedTypes.insert( "IHistogram1D" );
  m_supportedTypes.insert( "IHistogram2D" );
  m_supportedTypes.insert( "IHistogram3D" );
  m_supportedTypes.insert( "IProfile1D" );
  m_supportedTypes.insert( "IProfile2D" );
  m_supportedTypes.insert( "ITuple" );
}


bool
iAIDA::AIDA_RootStore::SupportedAIDATypes::isTypeSupported( const std::string& type ) const
{
  if ( m_supportedTypes.find( type ) == m_supportedTypes.end() ) return false;
  else return true;
}
