// Emacs -*- C++ -*-
#ifndef IAIDA_AIDA_TUPLE_DEVFILTERFACTORY
#define IAIDA_AIDA_TUPLE_DEVFILTERFACTORY 1

#include "AIDA_Dev/IDevFilterFactory.h"

namespace iAIDA {
  namespace AIDA_Tuple_native {
    class CompiledExpressionManager;

/// Implementation of the AIDA::Dev::IDevFilterFactory

class AIDA_DevFilterFactory : virtual public AIDA::Dev::IDevFilterFactory
{
public:
  /// Constructor
  AIDA_DevFilterFactory();

  /// Destructor
  ~AIDA_DevFilterFactory();

  /// Creates a filter given a C++ expression
  AIDA::IFilter* createScripted( const std::string & expression );

  /** Creates a filter given a C++ expression,
      the number of tuple rows to process and the starting row. */
  AIDA::IFilter* createScripted( const std::string & expression,
				 int rowsToProcess,
				 int startingRow = 0 );

private:
  CompiledExpressionManager*   m_mgr;

};

  }
}

#endif
