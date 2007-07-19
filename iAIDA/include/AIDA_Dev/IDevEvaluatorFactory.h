// Emacs -*- C++ -*-
#ifndef AIDA_IDEVEVALUATORFACTORY
#define AIDA_IDEVEVALUATORFACTORY 1

#include <string>

namespace AIDA {
  class IEvaluator;

  namespace Dev {

/// Abstract interface class for a factory which creates evaluator objects

class IDevEvaluatorFactory
{
public:
  /// Destructor
  virtual ~IDevEvaluatorFactory() {/*nop*/;}

  /// Creates an evaluator given a C++ expression
  virtual IEvaluator* createScripted( const std::string & expression ) = 0;
};

  }
}

#endif /* ifndef AIDA_IDEVEVALUATORFACTORY */
