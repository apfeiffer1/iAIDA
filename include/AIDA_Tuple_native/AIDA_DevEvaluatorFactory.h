// Emacs -*- C++ -*-
#ifndef IAIDA_AIDA_TUPLE_DEVEVALUATORFACTORY
#define IAIDA_AIDA_TUPLE_DEVEVALUATORFACTORY 1

#include "AIDA_Dev/IDevEvaluatorFactory.h"

namespace iAIDA {
  namespace AIDA_Tuple_native {
    class CompiledExpressionManager;

/// Implementation of the AIDA::Dev::IDevEvaluatorFactory

class AIDA_DevEvaluatorFactory : virtual public AIDA::Dev::IDevEvaluatorFactory
{
public:
  /// Constructor
  AIDA_DevEvaluatorFactory();

  /// Destructor
  ~AIDA_DevEvaluatorFactory();

  /// Creates a evaluator given a C++ expression
  AIDA::IEvaluator* createScripted( const std::string & expression );

private:
  CompiledExpressionManager*   m_mgr;

};

  }
}

#endif
