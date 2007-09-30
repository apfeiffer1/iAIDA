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
#ifndef IAIDA_AIDA_TUPLE_SYMBOL_MANAGER
#define IAIDA_AIDA_TUPLE_SYMBOL_MANAGER 1

#include <string>
#include <set>
#include <map>

namespace iAIDA {
  namespace AIDA_Tuple_native {
    class IEvaluatorExpressionBaseFactory;
    class IFilterExpressionBaseFactory;

class SymbolManager {
public:
  /// Constructor
  SymbolManager();
  /// Destructor
  ~SymbolManager();
  /// Sets the list of the include paths to be used for the compilation
  bool setIncludePaths( const std::set<std::string>& includes );
  /// Retrieves the list of the include paths to be used for the compilation
   const std::set<std::string>& includePaths() const;
  /// Compiles, builds and loads the factory of a filter expression
  IEvaluatorExpressionBaseFactory* makeAndLoadEvaluator( const std::string& fileName,
							 const std::string& symbolName );
  /// Compiles, builds and loads the factory of an evaluator expression
  IFilterExpressionBaseFactory* makeAndLoadFilter( const std::string& fileName,
						   const std::string& symbolName );
  /// Destroys the filter expression factory and unloads the objects
  bool destroyAndUnload( IEvaluatorExpressionBaseFactory* factory );
  /// Destroys the evaluator expression factory and unloads the objects
  bool destroyAndUnload( IFilterExpressionBaseFactory* factory );

private:
  /// The include paths
  std::set< std::string >                             m_includePaths;
  /// Maps of the library handles
  std::map< IEvaluatorExpressionBaseFactory*, void*>  m_evaluatorHandles;
  std::map< IFilterExpressionBaseFactory*, void*>     m_filterHandles;
};

  }
}

#endif /* ifndef IAIDA_AIDA_TUPLE_SYMBOL_GENERATOR */
