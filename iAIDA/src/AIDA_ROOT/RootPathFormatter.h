#ifndef IAIDA_AIDA_ROOTSTORE_ROOTPATHFORMATTER_H
#define IAIDA_AIDA_ROOTSTORE_ROOTPATHFORMATTER_H

#include <string>
#include <vector>

namespace iAIDA {
  namespace AIDA_RootStore {

class RootPathFormatter
{
public:
  static const RootPathFormatter& theFormatter();

protected:
  RootPathFormatter() {};
  RootPathFormatter( const RootPathFormatter&);
  RootPathFormatter& operator=( const RootPathFormatter&);

public:
  ~RootPathFormatter(){};

  // Decomposes a path
  std::vector< std::string > formPathNames( const std::string& path ) const;
};

  }
}

#include "RootPathFormatter.inl"

#endif
