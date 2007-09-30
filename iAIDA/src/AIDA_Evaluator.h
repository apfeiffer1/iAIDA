// Emacs -*- C++ -*-
#ifndef IAIDA_AIDA_TUPLE_AIDA_EVALUATOR
#define IAIDA_AIDA_TUPLE_AIDA_EVALUATOR 1

#include "AIDA/IEvaluator.h"
#include "AIDA_Tuple_native/IEvaluatorExpressionBase.h"
#include <memory>

namespace iAIDA {
  namespace AIDA_Tuple_native {
    class CompiledExpressionManager;

class AIDA_Evaluator : virtual public AIDA::IEvaluator
{
public:
  // Constructor
  AIDA_Evaluator( CompiledExpressionManager& manager,
		  const std::string& expression );
  // Destructor
  ~AIDA_Evaluator(){};

  // Methods inherited from AIDA/IEvaluator
  bool initialize( AIDA::ITuple & tuple );
  double evaluateDouble() const;
  std::string expression() const;

protected: 
  // for simple evaluations
  double m_simpleEvaluation() const; 
  // to clean up expression string 
  std::string m_cleanupExpression( ) const;

private:
  CompiledExpressionManager&                        m_manager;
  std::string                                       m_expression;
  std::auto_ptr<IEvaluatorExpressionBase>           m_evaluator;
  bool                                              m_simpleEvaluator;   // for siple cases : evaluator = variable  
  void  *                                           m_variableAdress;    // adress of the variable in the simpe cases
  std::string                                       m_variableType;      // variable Type for simple cases 
};

  }
}

#endif /* ifndef IAIDA_AIDA_TUPLE_AIDA_EVALUATOR */
