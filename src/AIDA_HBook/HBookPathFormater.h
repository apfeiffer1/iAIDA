#ifndef HBOOK_PATH_FORMATER_H
#define HBOOK_PATH_FORMATER_H

#include <string>
#include <vector>

namespace pi {
  namespace AIDA_HBookStore {

class HBookPathFormater
{
public:
  static const HBookPathFormater& theFormater();

protected:
  HBookPathFormater() {};
  HBookPathFormater( const HBookPathFormater&);
  HBookPathFormater& operator=( const HBookPathFormater&);

public:
  ~HBookPathFormater(){};

  // Decomposes a path
  std::vector< std::string > formPathNames( const std::string& path ) const;
};

  }
}

#include "HBookPathFormater.inl"

#endif
