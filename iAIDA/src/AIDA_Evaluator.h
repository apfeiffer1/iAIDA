//
//    Copyright 2007 Andreas Pfeiffer
//
/*
 *  This file is part of iAIDA.
 *
 *  iAIDA is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as 
 *  published by the Free Software Foundation, either version 3 of 
 *  the License.
 *
 *  iAIDA is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public 
 *  License along with iAIDA.  If not, see <http://www.gnu.org/licenses/>.
 */

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
