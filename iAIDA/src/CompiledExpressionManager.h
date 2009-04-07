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
#ifndef IAIDA_AIDA_TUPLE_COMPILED_EXPRESSION_MANAGER
#define IAIDA_AIDA_TUPLE_COMPILED_EXPRESSION_MANAGER 1

#include "AIDA_Tuple_native/IFilterExpressionBase.h"
#include "AIDA_Tuple_native/IEvaluatorExpressionBase.h"
#include <memory>
#include <map>
#include <set>
#include <vector>

namespace iAIDA {
  namespace AIDA_Tuple_native {
    class ExpressionParser;
    class SymbolManager;

class CompiledExpressionManager {
public:
  /// Constructor
  CompiledExpressionManager();
  /// Destructor
  ~CompiledExpressionManager();

  /// Returns a filter evaluation expression object given and expression and the variable types
  std::auto_ptr<IFilterExpressionBase> createFilterExpression( const std::string& expression,
							       const std::map<std::string, std::string>& variableTypes,
							       std::set<std::string>& variablesUsed );

  /// Returns a filter evaluation expression object given and expression and the variable types
  std::auto_ptr<IEvaluatorExpressionBase> createEvaluatorExpression( const std::string& expression,
								     const std::map<std::string, std::string>& variableTypes,
								     std::set<std::string>& variablesUsed );

  /// The default number of filter/evaluator objects in cache
  static const int defaultCacheSizeForFilterObjects = 100;
  static const int defaultCacheSizeForEvaluatorObjects = 100;

  /// Sets/Retrieves the cache size for the objects
  int cacheSizeForFilterObjects();
  int cacheSizeForEvaluatorObjects();
  void setCacheSizeForFilterObjects( int cacheSize );
  void setCacheSizeForEvaluatorObjects( int cacheSize );

  std::vector<std::string> findIncludePaths(const std::string & relName); 

private:
  int                 m_cacheSizeForFilterObjects;
  int                 m_cacheSizeForEvaluatorObjects;
  SymbolManager*      m_symbolManager;
  ExpressionParser*   m_expressionParser;

  std::map<std::string, IFilterExpressionBaseFactory*>    m_filterFactories;
  std::map<std::string, int>                              m_filterFactoryUsage;
  std::map<std::string, std::set<std::string> >           m_filterFactoryVariables;
  std::map<std::string, IEvaluatorExpressionBaseFactory*> m_evaluatorFactories;
  std::map<std::string, int>                              m_evaluatorFactoryUsage;
  std::map<std::string, std::set<std::string> >           m_evaluatorFactoryVariables;
};

  }
}

#endif /* ifndef IAIDA_AIDA_TUPLE_EXPRESSION_PARSER */
