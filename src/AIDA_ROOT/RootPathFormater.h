#ifndef PI_AIDA_ROOTSTORE_ROOTPATHFORMATER_H
#define PI_AIDA_ROOTSTORE_ROOTPATHFORMATER_H

#include <string>
#include <vector>

namespace pi {
  namespace AIDA_RootStore {

class RootPathFormater
{
public:
  static const RootPathFormater& theFormater();

protected:
  RootPathFormater() {};
  RootPathFormater( const RootPathFormater&);
  RootPathFormater& operator=( const RootPathFormater&);

public:
  ~RootPathFormater(){};

  // Decomposes a path
  std::vector< std::string > formPathNames( const std::string& path ) const;
};

  }
}

#include "RootPathFormater.inl"

#endif
