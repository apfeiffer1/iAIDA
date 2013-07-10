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
#ifndef IAIDA_AIDA_TUPLE_EVALUATOREXPRESSIONBASE
#define IAIDA_AIDA_TUPLE_EVALUATOREXPRESSIONBASE 1

#include <string>
#include <map>

namespace iAIDA {
  namespace AIDA_Tuple_native {

class IEvaluatorExpressionBase {
public:
  virtual ~IEvaluatorExpressionBase() {};
  virtual bool bind( const std::map<std::string, void*>& args ) = 0;
  virtual double evaluate() const = 0;
};

class IEvaluatorExpressionBaseFactory {
public:
  virtual ~IEvaluatorExpressionBaseFactory() {};
  virtual IEvaluatorExpressionBase* create() = 0;
};

template<class T>
class IEvaluatorExpressionFactory : virtual public IEvaluatorExpressionBaseFactory {
public:
  IEvaluatorExpressionFactory() {};
  ~IEvaluatorExpressionFactory() {};
  IEvaluatorExpressionBase* create() { return new T; }  
};

  }
}

#endif /* ifndef IAIDA_AIDA_TUPLE_EVALUATOREXPRESSIONBASE */
