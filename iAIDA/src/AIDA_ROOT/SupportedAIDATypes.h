#ifndef IAIDA_AIDA_ROOTSTORE_SUPPORTEDAIDATYPES_H
#define IAIDA_AIDA_ROOTSTORE_SUPPORTEDAIDATYPES_H

#include <string>
#include <set>

namespace iAIDA {
  namespace AIDA_RootStore {

class SupportedAIDATypes
{
public:
  static const SupportedAIDATypes& supportedTypes();

protected:
  SupportedAIDATypes();
  SupportedAIDATypes( const SupportedAIDATypes& );
  SupportedAIDATypes& operator=( const SupportedAIDATypes& );

public:
  ~SupportedAIDATypes() {}
  bool isTypeSupported( const std::string& type ) const;

private:
  std::set< std::string > m_supportedTypes;
};

  }
}

#endif