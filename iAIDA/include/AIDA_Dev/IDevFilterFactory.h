// Emacs -*- C++ -*-
#ifndef AIDA_IDEVFILTERFACTORY
#define AIDA_IDEVFILTERFACTORY 1

#include <string>

namespace AIDA {
  class IFilter;

  namespace Dev {

/// Abstract interface class for a factory which creates filter objects

class IDevFilterFactory
{
public:
  /// Destructor
  virtual ~IDevFilterFactory() {/*nop*/;}

  /// Creates a filter given a C++ expression
  virtual IFilter* createScripted( const std::string & expression ) = 0;

  /** Creates a filter given a C++ expression,
      the number of tuple rows to process and the starting row. */
  virtual IFilter* createScripted( const std::string & expression,
				   int rowsToProcess,
				   int startingRow = 0 ) = 0;
};

  }
}

#endif /* ifndef AIDA_IDEVFILTERFACTORY */
