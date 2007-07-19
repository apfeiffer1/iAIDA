#ifndef PI_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_SUPPORTEDAIDATYPES_H
#define PI_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_SUPPORTEDAIDATYPES_H

#include <string>
#include <set>

namespace pi {
  namespace AIDA_HBookStore {

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
