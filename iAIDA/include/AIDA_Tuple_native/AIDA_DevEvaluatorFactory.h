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
