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
#ifndef IAIDA_AIDA_TUPLE_EXPRESSION_PARSER
#define IAIDA_AIDA_TUPLE_EXPRESSION_PARSER 1

#include <string>
#include <iosfwd>
#include <map>
#include <set>

namespace iAIDA {
  namespace AIDA_Tuple_native {

class ExpressionParser {
public:
  /// Constructor
  ExpressionParser();
  /// Destructor
  ~ExpressionParser();

  /// Creates a function which evaluates an expression. Returns the variables used.
  std::set<std::string> mathematicalEvaluation( const std::string& expression,
						const std::map< std::string, std::string >& variablesToTypes,
						const std::string& functionName,
						std::ostream& os ) const;

  /// Creates a function which evaluates a logical expression
  std::set<std::string> logicalEvaluation( const std::string& expression,
					   const std::map< std::string, std::string >& variablesToTypes,
					   const std::string& functionName,
					   std::ostream& os ) const;

  /// Tells the system to include a set of header files in the generated expressions
  bool useHeaderFiles( const std::set< std::string >& headerFiles );

  /// Retrieves the currently used header files
  const std::set<std::string>& headerFilesUsed() const;

  /// Tells the system to use a set of namespaces
  bool useNamespaces( const std::set< std::string >& namespaces );

  /// Retrieves the currently used namespaces
  const std::set<std::string>& namespacesUsed() const;

private:
  /// header files to be included in the generated files
  std::set<std::string> m_headerFiles;

  /// namespaces to be used in the generated files
  std::set<std::string> m_namespaces;

  /// Checks which of the variables are used
  std::set<std::string> findUsedVariables( const std::string& expression,
					   const std::map< std::string, std::string >& variablesToTypes ) const;

  /// Checks if there is a variable in the expression
  bool existsVariableInExpression( const std::string& variable,
				   const std::string& expression ) const;

  /// Formats an expression in order to keep identation consistent
  std::string correctIdentation( const std::string& expression ) const;

};

  }
}

#endif /* ifndef IAIDA_AIDA_TUPLE_EXPRESSION_PARSER */
